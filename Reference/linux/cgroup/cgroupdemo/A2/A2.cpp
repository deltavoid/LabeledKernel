#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
using namespace std;

const string cpu_path = "/sys/fs/cgroup/cpu/test/";
const string cpu_quota_file = "cpu.cfs_quota_us";
const string cpuset_path = "/sys/fs/cgroup/cpuset/test/";
const string cpuset_cpus_file = "cpuset.cpus";
const string procs_file = "cgroup.procs";

int pid;
double cpu_rate;
string cpus;


void set_cpu_procs(int pid)
{
    //printf("%s\n", (cpu_path + procs_file).c_str());
    ofstream ofile((cpu_path + procs_file).c_str());
    
    ofile << pid << endl;

    ofile.close();
}

void set_cpu_quota(double rate)
{
    //printf("%s\n", (cpu_path + cpu_quota_file).c_str());
    ofstream ofile((cpu_path + cpu_quota_file).c_str());

    unsigned long long quota = (unsigned long long)(rate * 100000);
    ofile << quota << endl;

    ofile.close();
}

void set_cpuset_procs(int pid)
{
    //printf("%s\n", (cpuset_path + procs_file).c_str());
    ofstream ofile((cpuset_path + procs_file).c_str());

    ofile << pid << endl;

    ofile.close();
}

void set_cpuset_cpus(string cpus)
{
    //printf("%s\n", (cpuset_path + cpuset_cpus_file).c_str());
    ofstream ofile((cpuset_path + cpuset_cpus_file).c_str());

    ofile << cpus << endl;

    ofile.close();
}



int main(int argc, char** argv)
{
    for (int i = 1; i < argc; i++)
    {
        if  (strcmp(argv[i], "-cpu") == 0)
        {
            sscanf(argv[++i], "%lf", &cpu_rate);
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

    cout << pid << " " << cpu_rate << " " << cpus << endl;

    
    if  (cpu_rate > 0)
    {   set_cpu_procs(pid);
        set_cpu_quota(cpu_rate);
    }
    
    if  (cpus != "")
    {   set_cpuset_procs(pid);
        set_cpuset_cpus(cpus);
    }
    
    return 0;
}