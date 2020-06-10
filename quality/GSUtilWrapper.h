#pragma once
#include <string>
#include <vector>
#include "SystemWrapper.h"

class GSUtilWrapper
{
public:
	void SetBucket(std::string bucketname);
	void SetProject(std::string projectId);
	void ListFingerprintImages(std::vector<std::string> output); 
	void Download(std::string url);
	bool hasPermissions();
	GSUtilWrapper();
private:
	std::string _bucketname;
	std::string _projectId;
	SystemWrapper _system;
		
};

