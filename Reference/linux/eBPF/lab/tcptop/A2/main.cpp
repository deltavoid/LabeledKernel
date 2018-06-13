#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "network_monitor.h"

NetworkMonitor* monitor;

int main(int argc, char** argv)
{
    monitor = new NetworkMonitor();

    for (int i = 0; i < 100; i++)
    {
        std::cout << i << std::endl;
        sleep(1);
        for (auto it : monitor->process_send_bytes)
            std::cout << "pid: "<< it.first << " bytes: " << it.second << std::endl;
    }

    delete monitor;

}