#ifndef NETWORK_CONTROLLER_H
#define NETWORK_CONTROLLER_H
#include <string>

class NetworkController
{public:
    static const std::string cgroup_path;
    static const std::string procs_file;
    static const std::string classid_file;
    std::string LC_path;
    std::string BE_path;
    std::string device;

    NetworkController(std::string name, std::string _device, std::string rate);
    ~NetworkController();

    void set_LC_procs(int pid);
    void set_LC_bandwidth(std::string bw);
    void set_BE_procs(int pid);
    void set_BE_bandwidth(std::string bw);

};


#endif