#include "NameUtilities.h"

std::string getFilename(std::string path) {
	std::size_t found = path.find_last_of("/\\");
	return path.substr(found + 1);
}

std::string getFileType(std::string path) {
	std::size_t found = path.find_last_of(".");
	return path.substr(found + 1);
}