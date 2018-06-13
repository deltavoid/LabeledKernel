#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <iostream>
using namespace std;

const string cpu_path = "/sys/fs/cgroup/cpu/";
const string cpu_quota_file = "cpu.cfs_quota_us";
const string cpuset_path = "/sys/fs/cgroup/cpuset/";
const string cpuset_cpus_file = "cpuset.cpus";
const string procs_file = "cgroup.procs";
//const string tasks_file = "tasks";

/*void set_cpu_tasks(int pid)
{
}*/

void set_cpu_procs(int pid)
{
}

void set_cpu_quota(double rate)
{
}

void set_cpuset_tasks(int pid)
{
}

void set_cpuset_procs(int pid)
{
}

void set_cpuset_cpus(string cpus)
{
}

int pid;
bool proc_task;//proc:0, task:1
double cpu_rate;
string cpus;

int main(int argc, char** argv)
{


}