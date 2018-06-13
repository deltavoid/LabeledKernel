#!/usr/bin/python
#
# urandomread  Example of instrumenting a kernel tracepoint.
#              For Linux, uses BCC, BPF. Embedded C.
#
# REQUIRES: Linux 4.7+ (BPF_PROG_TYPE_TRACEPOINT support).
#
# Test by running this, then in another shell, run:
#     dd if=/dev/urandom of=/dev/null bs=1k count=5
#
# Copyright 2016 Netflix, Inc.
# Licensed under the Apache License, Version 2.0 (the "License")

from __future__ import print_function
from bcc import BPF

# define BPF program
bpf_text = """
#include <uapi/linux/ptrace.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <net/cls_cgroup.h>
#include <bcc/proto.h>

// static inline u32 task_get_classid(const struct sk_buff *skb)


struct info_t {
    //u32 pid;
    u32 saddr;
    u32 daddr;
    u16 lport;
    u16 dport;
};

TRACEPOINT_PROBE(net, net_dev_xmit) {

    struct sk_buff* skb = NULL;
    struct net_device* dev = NULL;
    struct sock* sk = NULL;
    struct info_t info = {};
    int len = 0;
    char name[16] = {0};
    u32 pid = bpf_get_current_pid_tgid();
    u16 family = 0;
    //u64 *val, zero = 0;

    skb = args->skbaddr;
    bpf_probe_read((void*)&len, sizeof(len), (void*)&skb->len);
    bpf_probe_read((void*)&dev, sizeof(dev), (void*)&skb->dev);
    bpf_probe_read((void*)&sk, sizeof(sk), (void*)&skb->sk);
    bpf_probe_read((void*)name, sizeof(name), (void*)dev->name);

    //family = sk->__sk_common.skc_family;
    bpf_probe_read((void*)&family, sizeof(family), (void*)&sk->__sk_common.skc_family);
    if (family == AF_INET) {
        //info.pid = pid;
        //info.saddr = sk->__sk_common.skc_rcv_saddr;
        bpf_probe_read((void*)&info.saddr, sizeof(info.saddr), (void*)&sk->__sk_common.skc_rcv_saddr);
        //info.daddr = sk->__sk_common.skc_daddr;
        bpf_probe_read((void*)&info.daddr, sizeof(info.daddr), (void*)&sk->__sk_common.skc_daddr);
        //info.lport = sk->__sk_common.skc_num;
        bpf_probe_read((void*)&info.lport, sizeof(info.lport), (void*)&sk->__sk_common.skc_num);
        //dport = sk->__sk_common.skc_dport;  info.dport = ntohs(dport);
        bpf_probe_read((void*)&info.dport, sizeof(info.dport), (void*)&sk->__sk_common.skc_dport);
        info.dport = ntohs(info.dport);




        //val = info.lookup_or_init(&ipv4_key, &zero);
        //(*val) += len;
    }
    


    bpf_trace_printk("%u %d %s \\n", pid, len, &name);
    bpf_trace_printk("%x %d \\n", info.saddr, (int)info.lport);
    bpf_trace_printk("%x %d \\n", info.daddr, (int)info.dport);
    bpf_trace_printk(" \\n");

    //bpf_trace_printk("%s \\n", &name);
    //bpf_trace_printk("%d \\n", *((int*)&name[0]));
    //bpf_trace_printk("%d \\n", *((int*)&name[4]));
    //bpf_trace_printk("%d \\n", *((int*)&name[8]));
    //bpf_trace_printk("%d \\n", *((int*)&name[12]));
    //bpf_trace_printk("%d %d\\n", *((int*)&name[0]), *((int*)&name[4]));
    
    return 0;
}
"""

# initialize BPF
b = BPF(text=bpf_text)



# header
print("%-18s %-16s %-6s %s" % ("TIME(s)", "COMM", "PID", "MSG"))

# format output
while 1:
    try:
        (task, pid, cpu, flags, ts, msg) = b.trace_fields()
    except ValueError:
        continue
    print("%-18.9f %-16s %-6d %s" % (ts, task, pid, msg))

print("over")
