#pragma once
#include <Windows.h>
#include <vector>
#include <string>

class FileWrapper
{
public:
	std::vector<unsigned char> getBinary(const char* filename);
	void writeFile(const char* filename, std::string input);
	bool getLines(std::string fileName, std::vector<std::string>& vecOfStrs);
};

