#pragma once
#include <vector>
#include <string>
class Image
{
public:
	void DecodeWsqFile(std::string filename, std::string* filename_out);

	void Downsize(const std::vector<unsigned char> input, std::vector<unsigned char>& output);

};

