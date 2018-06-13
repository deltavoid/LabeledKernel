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
from time import sleep, strftime

# define BPF program
bpf_text = """
#include <uapi/linux/ptrace.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <bcc/proto.h>

struct info_t
{   int len;
    char name[16];
};

BPF_HASH(info_set, struct info_t);

TRACEPOINT_PROBE(net, net_dev_xmit) {

    struct sk_buff* skb;
    struct net_device* dev;
    //int len;
    //char name[16];
    struct info_t info;
    u64 *val, zero= 0;
 
    skb = args->skbaddr;
    bpf_probe_read((void*)&info.len, sizeof(info.len), (void*)&skb->len);
    bpf_probe_read((void*)&dev, sizeof(dev), (void*)&skb->dev);
    bpf_probe_read((void*)info.name, sizeof(info.name), (void*)dev->name);

    //bpf_trace_printk("%d\\n", len);
    //bpf_trace_printk("%s \\n", name);
    //bpf_trace_printk("%d \\n", *((int*)&name[0]));
    //bpf_trace_printk("%d \\n", *((int*)&name[4]));
    //bpf_trace_printk("%d \\n", *((int*)&name[8]));
    //bpf_trace_printk("%d \\n", *((int*)&name[12]));
    //bpf_trace_printk("%d %d\\n", *((int*)&name[0]), *((int*)&name[4]));
    
    val = info_set.lookup_or_init(&info, &zero);

    return 0;
}
"""

# initialize BPF
b = BPF(text=bpf_text)
info_set = b["info_set"]


# header
print("%-18s %-16s %-6s %s %s" % ("TIME(s)", "COMM", "PID", "MSG", "MSG1"))

# format output
while 1:
    
    for k, v in info_set.items():
        print("%d %d" % k.len, str(k.name))
    
    sleep(1)

print("over")
