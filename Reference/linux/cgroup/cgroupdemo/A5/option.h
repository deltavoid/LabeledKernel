#ifndef OPTION_H
#define OPTION_H
#include <string>

class Option
{public:
    int pid;
    std::string cpus;
    std::string bandwidth;

    void parse(int argc, char** argv);
};

extern Option option;

#endif