#pragma once
#include <string>
#include "SystemWrapper.h"
class DirectoryWrapper
{
public:
	std::string ListFingerprintImages(std::string base_folder);
private:
	SystemWrapper _system;
	
};

