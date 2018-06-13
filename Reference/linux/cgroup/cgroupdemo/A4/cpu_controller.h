#ifndef CPU_CONTROLLER_H
#define CPU_CONTROLLER_H
#include <string>

class CpuController
{public:
    static const std::string LC_path;
    static const std::string BE_path;
    static const std::string quota_file;
    static const std::string procs_file;

    static void set_LC_procs(int pid);
    static void set_LC_quota(double rate);
    static void set_BE_procs(int pid);
    static void set_BE_quota(double rate);
    
};


#endif