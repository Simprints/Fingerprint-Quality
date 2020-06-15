#include "NameUtilities.h"

bool NameUtilities::getFilenameFromUrl(std::string url, std::string* filename) {
	std::size_t found = url.find_last_of("/\\");
	*filename = url.substr(found + 1);

	return (filename->find(".wsq") != std::string::npos);
}
