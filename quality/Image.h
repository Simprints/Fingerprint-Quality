#pragma once
#include <vector>
#include <string>
class Image
{
public:
	void DecodeWsq(std::vector<unsigned char> input, unsigned char** output);
	void DecodeWsqFile(std::string filename, std::string* filename_out);
	//void Downsize(const std::vector<BYTE>& input, std::vector<BYTE>& output);
};

