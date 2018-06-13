#ifndef CPUSET_CONTROLLER_H
#define CPUSET_CONTROLLER_H
#include <string>
#include <vector>

class CpusetController
{public:
    static const std::string cgroup_path;
    static const std::string procs_file;
    static const std::string cpus_file;
    std::string LC_path;
    std::string BE_path;
    

    CpusetController(std::string name);
    ~CpusetController();

    void set_LC_procs(int pid);
    void set_LC_cpus(std::string cpus);
    void set_BE_procs(int pid);
    void set_BE_cpus(std::string cpus);

};


#endif