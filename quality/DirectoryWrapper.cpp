#include "DirectoryWrapper.h"

#include <iostream>

std::string DirectoryWrapper::ListFingerprintImages(std::string base_folder)
{
	std::cout << "Fetching list of fingerprint images..." << std::endl;
	return _system.run("cd " + base_folder + "& dir *.raw *.wsq *.pgm /S /B");

}
