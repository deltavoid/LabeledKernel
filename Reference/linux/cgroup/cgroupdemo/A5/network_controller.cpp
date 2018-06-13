#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include "network_controller.h"
using namespace std;


const string NetworkController::cgroup_path = "/sys/fs/cgroup/net_cls/";
const string NetworkController::procs_file = "cgroup.procs";
const string NetworkController::classid_file = "net_cls.classid";

NetworkController::NetworkController(string name, string _device, string rate)
{
    device = _device;
    LC_path = cgroup_path + name + string("/LC/");
    BE_path = cgroup_path + name + string("/BE/");
    system((string("mkdir ") + cgroup_path + name).c_str());
    system((string("mkdir ") + LC_path).c_str());
    system((string("mkdir ") + BE_path).c_str());
    system((string("echo '0x10003' > ") + LC_path + classid_file).c_str());
    system((string("echo '0x10004' > ") + BE_path + classid_file).c_str());

    system((string("tc qdisc del dev ") + device + string(" root")).c_str());
    system((string("tc qdisc add dev ") + device + string(" root handle 1: htb")).c_str());
    system((string("tc class add dev ") + device + string(" parent 1: classid 1: htb rate ") + rate + string(" ceil ") + rate).c_str());
    system((string("tc class add dev ") + device + string(" parent 1: classid 1:3 htb rate ") + rate).c_str());
    system((string("tc class add dev ") + device + string(" parent 1: classid 1:4 htb rate ") + rate).c_str());
    system((string("tc filter add dev ") + device + string(" protocol ip parent 1:0 prio 1 handle 1: cgroup")).c_str());

}

NetworkController::~NetworkController()
{
}

void NetworkController::set_LC_procs(int pid)
{
    ofstream ofile((LC_path + procs_file).c_str());

    ofile << pid << endl;

    ofile.close();
}

void NetworkController::set_LC_bandwidth(string bw)
{
    system((string("tc class change dev ") + device + string(" parent 1: classid 1:3 htb rate ") + bw).c_str());
}

void NetworkController::set_BE_procs(int pid)
{
    ofstream ofile((BE_path + procs_file).c_str());

    ofile << pid << endl;

    ofile.close();
}

void NetworkController::set_BE_bandwidth(string bw)
{
    cout << "bandwidth: " << bw << endl;
    system((string("tc class change dev ") + device + string(" parent 1: classid 1:4 htb rate ") + bw).c_str());
}