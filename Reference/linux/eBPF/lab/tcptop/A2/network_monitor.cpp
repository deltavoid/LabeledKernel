#include "network_monitor.h"
#include <iostream>

const std::string NetworkMonitor::BPF_PROGRAM = R"(
#include <uapi/linux/ptrace.h>
#include <net/sock.h>
#include <bcc/proto.h>

struct ipv4_key_t {
    u32 pid;
    u32 saddr;
    u32 daddr;
    u16 lport;
    u16 dport;
};
BPF_HASH(ipv4_send_bytes, struct ipv4_key_t);
BPF_HASH(ipv4_recv_bytes, struct ipv4_key_t);

struct ipv6_key_t {
    u32 pid;
    // workaround until unsigned __int128 support:
    u64 saddr0;
    u64 saddr1;
    u64 daddr0;
    u64 daddr1;
    u16 lport;
    u16 dport;
};
BPF_HASH(ipv6_send_bytes, struct ipv6_key_t);
BPF_HASH(ipv6_recv_bytes, struct ipv6_key_t);

int kprobe__tcp_sendmsg(struct pt_regs *ctx, struct sock *sk,
    struct msghdr *msg, size_t size)
{
    u32 pid = bpf_get_current_pid_tgid();
    u16 dport = 0, family = sk->__sk_common.skc_family;
    u64 *val, zero = 0;

    if (family == AF_INET) {
        struct ipv4_key_t ipv4_key = {.pid = pid};
        ipv4_key.saddr = sk->__sk_common.skc_rcv_saddr;
        ipv4_key.daddr = sk->__sk_common.skc_daddr;
        ipv4_key.lport = sk->__sk_common.skc_num;
        dport = sk->__sk_common.skc_dport;
        ipv4_key.dport = ntohs(dport);
        val = ipv4_send_bytes.lookup_or_init(&ipv4_key, &zero);
        (*val) += size;

    } else if (family == AF_INET6) {
        struct ipv6_key_t ipv6_key = {.pid = pid};

        bpf_probe_read(&ipv6_key.saddr0, sizeof(ipv6_key.saddr0),
            &sk->__sk_common.skc_v6_rcv_saddr.in6_u.u6_addr32[0]);
        bpf_probe_read(&ipv6_key.saddr1, sizeof(ipv6_key.saddr1),
            &sk->__sk_common.skc_v6_rcv_saddr.in6_u.u6_addr32[2]);
        bpf_probe_read(&ipv6_key.daddr0, sizeof(ipv6_key.daddr0),
            &sk->__sk_common.skc_v6_daddr.in6_u.u6_addr32[0]);
        bpf_probe_read(&ipv6_key.daddr1, sizeof(ipv6_key.daddr1),
            &sk->__sk_common.skc_v6_daddr.in6_u.u6_addr32[2]);
        ipv6_key.lport = sk->__sk_common.skc_num;
        dport = sk->__sk_common.skc_dport;
        ipv6_key.dport = ntohs(dport);
        val = ipv6_send_bytes.lookup_or_init(&ipv6_key, &zero);
        (*val) += size;
    }
    // else drop

    return 0;
}

/*
 * tcp_recvmsg() would be obvious to trace, but is less suitable because:
 * - we'd need to trace both entry and return, to have both sock and size
 * - misses tcp_read_sock() traffic
 * we'd much prefer tracepoints once they are available.
 */
int kprobe__tcp_cleanup_rbuf(struct pt_regs *ctx, struct sock *sk, int copied)
{
    u32 pid = bpf_get_current_pid_tgid();
    u16 dport = 0, family = sk->__sk_common.skc_family;
    u64 *val, zero = 0;

    if (copied <= 0)
        return 0;

    if (family == AF_INET) {
        struct ipv4_key_t ipv4_key = {.pid = pid};
        ipv4_key.saddr = sk->__sk_common.skc_rcv_saddr;
        ipv4_key.daddr = sk->__sk_common.skc_daddr;
        ipv4_key.lport = sk->__sk_common.skc_num;
        dport = sk->__sk_common.skc_dport;
        ipv4_key.dport = ntohs(dport);
        val = ipv4_recv_bytes.lookup_or_init(&ipv4_key, &zero);
        (*val) += copied;

    } else if (family == AF_INET6) {
        struct ipv6_key_t ipv6_key = {.pid = pid};
        bpf_probe_read(&ipv6_key.saddr0, sizeof(ipv6_key.saddr0),
            &sk->__sk_common.skc_v6_rcv_saddr.in6_u.u6_addr32[0]);
        bpf_probe_read(&ipv6_key.saddr1, sizeof(ipv6_key.saddr1),
            &sk->__sk_common.skc_v6_rcv_saddr.in6_u.u6_addr32[2]);
        bpf_probe_read(&ipv6_key.daddr0, sizeof(ipv6_key.daddr0),
            &sk->__sk_common.skc_v6_daddr.in6_u.u6_addr32[0]);
        bpf_probe_read(&ipv6_key.daddr1, sizeof(ipv6_key.daddr1),
            &sk->__sk_common.skc_v6_daddr.in6_u.u6_addr32[2]);
        ipv6_key.lport = sk->__sk_common.skc_num;
        dport = sk->__sk_common.skc_dport;
        ipv6_key.dport = ntohs(dport);
        val = ipv6_recv_bytes.lookup_or_init(&ipv6_key, &zero);
        (*val) += copied;
    }
    // else drop

    return 0;
}
)";


NetworkMonitor::NetworkMonitor()
{
    auto init_res = bpf.init(BPF_PROGRAM);
    if (init_res.code() != 0)
    {
        std::cerr << init_res.msg() << std::endl;
        return;
    }

    auto attach_res = bpf.attach_kprobe("tcp_sendmsg", "kprobe__tcp_sendmsg");
    if (attach_res.code() != 0)
    {
        std::cerr << attach_res.msg() << std::endl;
        return;
    }

    int thread_res = pthread_create(&run_thread, NULL, run, (void*)this);


}

NetworkMonitor::~NetworkMonitor()
{
    auto detach_res = bpf.detach_kprobe("tcp_sendmsg");
    if (detach_res.code() != 0)
    {
        std::cerr << detach_res.msg() << std::endl;
        return;
    }
}

double NetworkMonitor::get_process_tx(int pid)
{
    if  (process_send_bytes.find(pid) != process_send_bytes.end())
        return (double)process_send_bytes[pid];
    return 0;
}

void* NetworkMonitor::run(void* arg)
{
    NetworkMonitor* This = (NetworkMonitor*) arg;

    while (true)
    {
        sleep(1);
        This->ipv4_send_bytes = This->bpf.get_hash_table<ipv4_key_t, u64>("ipv4_send_bytes").get_table_offline();
        This->bpf.get_hash_table<ipv4_key_t, u64>("ipv4_send_bytes").clear_table_non_atomic();

        This->process_send_bytes.clear();
        for (auto it : This->ipv4_send_bytes)
        {
            This->process_send_bytes[it.first.pid] += it.second;
        }
    }
}

