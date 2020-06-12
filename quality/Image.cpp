#include "Image.h"
#include "SystemWrapper.h"
#include <iostream>


#define cimg_display 0
#include "lib/CImg/CImg.h"
using namespace cimg_library;

const std::string DWSQ("C:\\dev\\Fingerprint-Quality\\quality\\lib\\wsq\\dwsq.exe");

void Image::DecodeWsq(std::vector<unsigned char> input, unsigned char** output)
{
	SystemWrapper system;
	std::cout << system.run("C:\\dev\\Fingerprint-Quality\\quality\\lib\\wsq\\dwsq.exe");

}

void Image::DecodeWsqFile(std::string input, std::string output)
{
	SystemWrapper system;
	std::cout << system.run(DWSQ);

}

//void Image::Downsize(const std::vector<BYTE>& input, std::vector<BYTE>& output)
//{
//}
