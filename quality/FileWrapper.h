#pragma once
#include <Windows.h>
#include <vector>
#include <string>

class FileWrapper
{
public:
	std::vector<unsigned char> readFile(const char* filename);
	void writeFile(const char* filename, std::string input);
};

