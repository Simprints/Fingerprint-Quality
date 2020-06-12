#include "FileWrapper.h"
#include <fstream>
#include <iostream>

std::vector<unsigned char> FileWrapper::getBinary(const char* filename)
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

	std::vector<unsigned char> vec;
	vec.reserve(fileSize);

	vec.insert(vec.begin(),
		std::istream_iterator<unsigned char>(file),
		std::istream_iterator<unsigned char>());

	return vec;
}

void FileWrapper::writeFile(const char* filename, std::string input) {
	std::cout << "Writing to " << filename << std::endl;
	std::ofstream out(filename);
	out << input;
	out.close();
}

bool FileWrapper::getLines(std::string fileName, std::vector<std::string>& vecOfStrs)
{

	// Open the File
	std::ifstream in(fileName.c_str());

	// Check if object is valid
	if (!in)
	{
		std::cerr << "Cannot open the File : " << fileName << std::endl;
		return false;
	}

	std::string str;
	// Read the next line from File untill it reaches the end.
	while (std::getline(in, str))
	{
		// Line contains string of length > 0 then save it in vector
		if (str.size() > 0)
			vecOfStrs.push_back(str);
	}
	//Close The File
	in.close();
	return true;
}

