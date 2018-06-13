#ifndef CPUSET_CONTROLLER_H
#define CPUSET_CONTROLLER_H
#include <string>

class CpusetController
{public:
    static const std::string LC_path;
    static const std::string BE_path;
    static const std::string cpus_file;
    static const std::string procs_file;

    static void set_LC_procs(int pid);
    static void set_LC_cpus(std::string cpus);
    static void set_BE_procs(int pid);
    static void set_BE_cpus(std::string cpus);
    
};

#endif