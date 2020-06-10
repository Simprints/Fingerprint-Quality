#include "SystemWrapper.h"
#include <iostream>

int SystemWrapper::run(std::string cmd)
{
    return system(cmd.c_str());
}

SystemWrapper::SystemWrapper()
{
    if (!system(NULL))
        std::cout << "Error: Command processor doesn't exist";
}
