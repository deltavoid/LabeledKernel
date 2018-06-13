#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "cpuset_controller.h"
using namespace std;

const string CpusetController::cgroup_path = "/sys/fs/cgroup/cpuset/";
const string CpusetController::procs_file = "cgroup.procs";
const string CpusetController::cpus_file = "cpuset.cpus";


CpusetController::CpusetController(string name)
{
    LC_path = cgroup_path + name + string("/LC/");
    BE_path = cgroup_path + name + string("/BE/");

    system((string("mkdir ") + cgroup_path + name).c_str());
    system((string("mkdir ") + LC_path).c_str());
    system((string("mkdir ") + BE_path).c_str());
}

CpusetController::~CpusetController()
{
}


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