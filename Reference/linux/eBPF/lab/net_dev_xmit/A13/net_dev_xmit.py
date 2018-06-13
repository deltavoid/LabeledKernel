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
from socket import inet_ntop, AF_INET, AF_INET6
from struct import pack
from time import sleep, strftime
#from subprocess import call
import ctypes as ct


# define BPF program
bpf_text = """
#include <uapi/linux/ptrace.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <net/cls_cgroup.h>
#include <bcc/proto.h>



struct info_t {
    //u32 pid;
    u32 classid;

    char name0;
    char name1;
    char name2;
    char name3;

    //u32 saddr;
    //u32 daddr;
    //u16 lport;
    //u16 dport;
};

BPF_HASH(info_set, struct info_t);

TRACEPOINT_PROBE(net, net_dev_xmit) {

    struct sk_buff* skb = NULL;
    struct net_device* dev = NULL;
    struct sock* sk = NULL;
    struct sock_cgroup_data* skcd = NULL;
    u8 skcd_is_data = 0;
    u32 skcd_classid = 0;
    u32 classid = 0;
    struct info_t info = {};
    int len = 0;
    char name[16] = {0};
    u32 pid = bpf_get_current_pid_tgid();
    u16 family = 0;
    u64 *val, zero = 0;

    skb = args->skbaddr;
    bpf_probe_read((void*)&len, sizeof(len), (void*)&skb->len);
    bpf_probe_read((void*)&dev, sizeof(dev), (void*)&skb->dev);
    bpf_probe_read((void*)&sk, sizeof(sk), (void*)&skb->sk);
    bpf_probe_read((void*)name, sizeof(name), (void*)dev->name);
    skcd = &sk->sk_cgrp_data;
    bpf_probe_read((void*)&skcd_is_data, sizeof(skcd_is_data), (void*)&skcd->is_data);
    bpf_probe_read((void*)&skcd_classid, sizeof(skcd_classid), (void*)&skcd->classid);
    classid = (skcd_is_data & 1) ? skcd_classid : 0;

    //info.pid = pid;
    info.classid = classid;


    info.name0 = name[0];
    info.name1 = name[1];
    info.name2 = name[2];
    info.name3 = name[3];

    /*bpf_probe_read((void*)&family, sizeof(family), (void*)&sk->__sk_common.skc_family);
    if (family == AF_INET) {
        bpf_probe_read((void*)&info.saddr, sizeof(info.saddr), (void*)&sk->__sk_common.skc_rcv_saddr);
        bpf_probe_read((void*)&info.daddr, sizeof(info.daddr), (void*)&sk->__sk_common.skc_daddr);
        bpf_probe_read((void*)&info.lport, sizeof(info.lport), (void*)&sk->__sk_common.skc_num);
        bpf_probe_read((void*)&info.dport, sizeof(info.dport), (void*)&sk->__sk_common.skc_dport);
        info.dport = ntohs(info.dport);
    
    }*/

    val = info_set.lookup_or_init(&info, &zero);
    (*val) += len;
    
    
    return 0;
}
"""

# initialize BPF
b = BPF(text=bpf_text)
info_set = b["info_set"]


def pid_to_comm(pid):
    try:
        comm = open("/proc/%d/comm" % pid, "r").read().rstrip()
        return comm
    except IOError:
        return str(pid)


while True:

    for k, v in info_set.items():
        #print("%s %x %d %x %d %s" % str(k.name0), k.saddr, k.lport, k.daddr, k.dport, str(v))
        #print("%d %s %d %s" % k.saddr, str(k.lport), k.daddr, str(k.dport))
        #print("%s" % k.name0)
        #print(k.saddr)
        #print(k.lport)
        #print(k.daddr)
        #print(k.dport)
        #print(v.value)
        #print()
        print("%c%c%c%c  %-6x %12s" % (k.name0, k.name1, k.name2, k.name3,
            k.classid,
            v.value))
    
    print()
    
    
    info_set.clear()
    sleep(1)

print("over")
