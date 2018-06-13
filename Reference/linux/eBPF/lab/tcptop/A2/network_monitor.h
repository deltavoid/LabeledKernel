#ifndef NETWORK_MONITOR_H
#define NETWORK_MONITOR_H

#include <string>
#include <vector>
#include <map>
#include <pthread.h>
#include "BPF.h"

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned long long u64;

struct ipv4_key_t {
    u32 pid;
    u32 saddr;
    u32 daddr;
    u16 lport;
    u16 dport;
};

class NetworkMonitor
{public:
    
    NetworkMonitor();
    ~NetworkMonitor();

    double get_process_tx(int pid);

    static void* run(void* arg);

    ebpf::BPF bpf;
    static const std::string BPF_PROGRAM;
    pthread_t run_thread;
    std::vector<std::pair<ipv4_key_t, u64> > ipv4_send_bytes;
    std::map<int, u64> process_send_bytes;

};


#endif