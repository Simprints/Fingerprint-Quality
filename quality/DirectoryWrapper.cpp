#include "DirectoryWrapper.h"
#include "SystemWrapper.h"
#include <iostream>

std::string ListFingerprintImages(std::string base_folder)
{
	std::cout << "Fetching list of fingerprint images..." << std::endl;
	SystemWrapper _system;
	return _system.run("cd " + base_folder + "& dir *.raw *.wsq *.pgm /S /B");
}
