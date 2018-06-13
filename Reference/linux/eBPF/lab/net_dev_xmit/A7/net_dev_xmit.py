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
#include <bcc/proto.h>

TRACEPOINT_PROBE(net, net_dev_xmit) {

    struct sk_buff* skb = NULL;
    struct net_device* dev = NULL;
    int len = 0;
    char name[16] = {0,};

    skb = args->skbaddr;
    bpf_probe_read((void*)&len, sizeof(len), (void*)&skb->len);
    bpf_probe_read((void*)&dev, sizeof(dev), (void*)&skb->dev);
    bpf_probe_read((void*)name, sizeof(name), (void*)dev->name);

    bpf_trace_printk("%d %s\\n", len, &name);

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
