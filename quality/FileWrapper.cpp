#include "FileWrapper.h"
#include <fstream>
#include <iostream>

std::vector<BYTE> FileWrapper::readFile(const char* filename)
{
	std::ifstream file(filename, std::ios::binary);
	if (!file.is_open()) {
		std::cout << "Cannot open file" << std::endl;
		return {};
	}

	// Stop eating new lines in binary mode
	file.unsetf(std::ios::skipws);

	std::streampos fileSize;

	file.seekg(0, std::ios::end);
	fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<BYTE> vec;
	vec.reserve(fileSize);

	vec.insert(vec.begin(),
		std::istream_iterator<BYTE>(file),
		std::istream_iterator<BYTE>());

	return vec;
}