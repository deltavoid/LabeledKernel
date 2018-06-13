#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include "cpu_controller.h"
using namespace std;

const string CpuController::LC_path = "/sys/fs/cgroup/cpu/LC/";
const string CpuController::BE_path = "/sys/fs/cgroup/cpu/BE/";
const string CpuController::quota_file = "cpu.cfs_quota_us";
const string CpuController::procs_file = "cgroup.procs";

void CpuController::set_LC_procs(int pid)
{
    ofstream ofile((LC_path + procs_file).c_str());

    ofile << pid << endl;

    ofile.close();
}

void CpuController::set_LC_quota(double rate)
{
    ofstream ofile((LC_path + quota_file).c_str());

    unsigned long long quota = (unsigned long long)(rate * 100000);
    ofile << quota << endl;

    ofile.close();
}

void CpuController::set_BE_procs(int pid)
{
    printf("%s\n", (BE_path + procs_file).c_str());
    ofstream ofile((BE_path + procs_file).c_str());

    ofile << pid << endl;

    ofile.close();
}

void CpuController::set_BE_quota(double rate)
{
    printf("%s\n", (BE_path + quota_file).c_str());
    ofstream ofile((BE_path + quota_file).c_str());

    unsigned long long quota = (unsigned long long)(rate * 100000);
    ofile << quota << endl;

    ofile.close();
}