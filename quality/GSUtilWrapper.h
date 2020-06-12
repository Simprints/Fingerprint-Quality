#pragma once
#include <string>
#include <vector>
#include "SystemWrapper.h"

class GsutilWrapper
{
public:
	void SetBucket(std::string bucketname);
	void SetProject(std::string projectId);
	std::string ListFingerprintImages();
	void Download(std::string url, std::string destination);
	bool getFilenameFromUrl(std::string url, std::string* filename);
	GsutilWrapper();
private:
	std::string _bucketname;
	std::string _projectId;
	SystemWrapper _system;
		
};

