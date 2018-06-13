#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <fstream>
#include "option.h"
#include "network_driver.h"

Option option;
NetworkDriver network_driver;


int main(int argc, char** argv)
{
    option.parse(argc, argv);
    network_driver = new NetworkDriver("heracles", "eno1", 1e9 / 8);


    if  (option.bandwidth != "")
    {
        networkController->set_BE_procs(option.pid);
        networkController->set_BE_bandwidth(option.bandwidth);
    }

    delete network_driver;

    return 0;
}
