#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include "cpuset_controller.h"
using namespace std;


const string CpusetController::LC_path = "/sys/fs/cgroup/cpuset/LC/";
const string CpusetController::BE_path = "/sys/fs/cgroup/cpuset/BE/";
const string CpusetController::cpus_file = "cpuset.cpus";
const string CpusetController::procs_file = "cgroup.procs";

void CpusetController::set_LC_procs(int pid)
{
    ofstream ofile((LC_path + procs_file).c_str());

    ofile << pid << endl;

    ofile.close();
}

void CpusetController::set_LC_cpus(string cpus)
{
    ofstream ofile((LC_path + cpus_file).c_str());

    ofile << cpus << endl;

    ofile.close();
}

void CpusetController::set_BE_procs(int pid)
{
    ofstream ofile((BE_path + procs_file).c_str());

    ofile << pid << endl;

    ofile.close();
}

void CpusetController::set_BE_cpus(string cpus)
{
    ofstream ofile((BE_path + cpus_file).c_str());

    ofile << cpus << endl;

    ofile.close();
}