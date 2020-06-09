#pragma once
#include <Windows.h>
#include <vector>
class FileWrapper
{
public:
	std::vector<BYTE> readFile(const char* filename);
};

