#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include "cpu_controller.h"
#include "cpuset_controller.h"
using namespace std;

int pid;
double rate;
std::string cpus;


int main(int argc, char** argv)
{
    for (int i = 1; i < argc; i++)
    {
        if  (strcmp(argv[i], "-cpu") == 0)
        {
            sscanf(argv[++i], "%lf", &rate);
        }
        else if  (strcmp(argv[i], "-cpuset") == 0)
        {
            cpus = argv[++i];
        }
        else
        {
            sscanf(argv[i], "%d", &pid);
        }
    }

    cout << pid << " " << rate << " " << cpus << endl; 

    if  (rate > 0)
    {
        CpuController::set_BE_procs(pid);
        CpuController::set_BE_quota(rate);
    }

    if  (cpus != "")
    {
        CpusetController::set_BE_procs(pid);
        CpusetController::set_BE_cpus(cpus);
    }

    return 0;
}