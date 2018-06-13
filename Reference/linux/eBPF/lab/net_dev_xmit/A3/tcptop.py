#!/usr/bin/python

from __future__ import print_function
from bcc import BPF
import argparse
from socket import inet_ntop, AF_INET, AF_INET6
from struct import pack
from time import sleep, strftime
from subprocess import call
import ctypes as ct



# linux stats
loadavg = "/proc/loadavg"

# define BPF program
# void dev_queue_xmit_nit(struct sk_buff *skb, struct net_device *dev);
bpf_text = """
#include <uapi/linux/ptrace.h>
#include <net/sock.h>
#include <net/skbuff.h>
#include <bcc/proto.h>

struct ipv4_key_t {
    u32 pid;
    u32 saddr;
    u32 daddr;
    u16 lport;
    u16 dport;
};
//BPF_HASH(ipv4_send_bytes, struct ipv4_key_t);
//BPF_HASH(ipv4_recv_bytes, struct ipv4_key_t);

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
//BPF_HASH(ipv6_send_bytes, struct ipv6_key_t);
//BPF_HASH(ipv6_recv_bytes, struct ipv6_key_t);

int kprobe__dev_queue_xmit_nit(struct pt_regs *ctx, struct sk_buff *skb, struct net_device *dev)
{
    u32 pid = bpf_get_current_pid_tgid();
    //u32 len = skb->len;

    
    bpf_trace_printk("%d \\n", skb->len);
    bpf_trace_printk("%s \\n", dev->name);

    /*
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
    */

    return 0;
}

"""


# initialize BPF
b = BPF(text=bpf_text)

ipv4_send_bytes = b["ipv4_send_bytes"]
ipv4_recv_bytes = b["ipv4_recv_bytes"]
ipv6_send_bytes = b["ipv6_send_bytes"]
ipv6_recv_bytes = b["ipv6_recv_bytes"]

print('Tracing... Output every %s secs. Hit Ctrl-C to end' % args.interval)

# output
i = 0
exiting = False
while i != args.count and not exiting:
    try:
        sleep(args.interval)
    except KeyboardInterrupt:
        exiting = True

    # header
    if args.noclear:
        print()
    else:
        call("clear")
    if not args.nosummary:
        with open(loadavg) as stats:
            print("%-8s loadavg: %s" % (strftime("%H:%M:%S"), stats.read()))

    # IPv4:  build dict of all seen keys
    keys = ipv4_recv_bytes
    for k, v in ipv4_send_bytes.items():
        if k not in keys:
            keys[k] = v

    if keys:
        print("%-6s %-12s %-21s %-21s %6s %6s" % ("PID", "COMM",
            "LADDR", "RADDR", "RX_KB", "TX_KB"))

    # output
    for k, v in reversed(sorted(keys.items(), key=lambda keys: keys[1].value)):
        send_kbytes = 0
        if k in ipv4_send_bytes:
            send_kbytes = int(ipv4_send_bytes[k].value / 1024)
        recv_kbytes = 0
        if k in ipv4_recv_bytes:
            recv_kbytes = int(ipv4_recv_bytes[k].value / 1024)

        print("%-6d %-12.12s %-21s %-21s %6d %6d" % (k.pid,
            pid_to_comm(k.pid),
            inet_ntop(AF_INET, pack("I", k.saddr)) + ":" + str(k.lport),
            inet_ntop(AF_INET, pack("I", k.daddr)) + ":" + str(k.dport),
            recv_kbytes, send_kbytes))

    ipv4_send_bytes.clear()
    ipv4_recv_bytes.clear()

    # IPv6: build dict of all seen keys
    keys = ipv6_recv_bytes
    for k, v in ipv6_send_bytes.items():
        if k not in keys:
            keys[k] = v

    if keys:
        # more than 80 chars, sadly.
        print("\n%-6s %-12s %-32s %-32s %6s %6s" % ("PID", "COMM",
            "LADDR6", "RADDR6", "RX_KB", "TX_KB"))

    # output
    for k, v in reversed(sorted(keys.items(), key=lambda keys: keys[1].value)):
        send_kbytes = 0
        if k in ipv6_send_bytes:
            send_kbytes = int(ipv6_send_bytes[k].value / 1024)
        recv_kbytes = 0
        if k in ipv6_recv_bytes:
            recv_kbytes = int(ipv6_recv_bytes[k].value / 1024)

        print("%-6d %-12.12s %-32s %-32s %6d %6d" % (k.pid,
            pid_to_comm(k.pid),
            inet_ntop(AF_INET6, pack("QQ", k.saddr0, k.saddr1)) + ":" +
            str(k.lport),
            inet_ntop(AF_INET6, pack("QQ", k.daddr0, k.daddr1)) + ":" +
            str(k.dport),
            recv_kbytes, send_kbytes))

    ipv6_send_bytes.clear()
    ipv6_recv_bytes.clear()

    i += 1
