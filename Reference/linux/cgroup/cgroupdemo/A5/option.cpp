#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <fstream>
#include "option.h"
using namespace std;

/*
    ./main <pid>
        -cpus <cpus>
        -bandwidth <bandwidth>
*/

void Option::parse(int argc, char** argv)
{
    for (int i = 1; i < argc; i++)
    {
        if  (strcmp(argv[i], "-cpuset") == 0)
        {
            cpus = argv[++i];
        }
        else if  (strcmp(argv[i], "-bandwidth") == 0)
        {
            bandwidth = argv[++i];
        }
        else
        {
            sscanf(argv[i], "%d", &pid);
        }
    }

    cout << "pid: " << pid << " "
         << "cpus: " << cpus << " "
         << "bandwitdh: " << bandwidth << endl;

}