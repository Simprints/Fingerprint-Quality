#pragma once
#include <Windows.h>
#include <vector>
#include <string>

class FileWrapper
{
public:
	std::vector<unsigned char> getBinary(const char* filename);
	void writeFile(const char* filename, std::string input);
	void writePairsFile(const char* filename, std::vector<std::pair<std::string, unsigned int>> input);
	bool getLines(std::string fileName, std::vector<std::string>& vecOfStrs);
};

