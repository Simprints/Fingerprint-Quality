#pragma once
#include <vector>
#include <string>
class Image
{
public:
	std::string Decode();
	void Downsize(const std::vector<unsigned char> input, std::vector<unsigned char>& output);
	Image(std::string path);

private:
	std::string _path;
	std::string _decodedFilePath;
	void DecodeWsqFile(std::string filename, std::string* filename_out);

};

