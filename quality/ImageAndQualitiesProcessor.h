#pragma once
#include "NameUtilities.h"
#include "Image.h"
#include "FileWrapper.h"
#include "SecugenWrapper.h"
#include <mutex>
#include <atomic>

class ImageAndQualitiesProcessor
{
private:
	NameUtilities name;
	Image image;
	FileWrapper files;
	SecugenWrapper secugen; 
	static std::mutex ReadLock;
	static std::mutex WriteLock;
	std::vector<std::string>& _fingerprintsUrls;
	static std::atomic<unsigned int> counter;

public:
	std::string FetchImageUrl();
	unsigned int ProcessImage(std::string url);
	void UploadResults(std::string url, unsigned int quality);
	ImageAndQualitiesProcessor(std::vector<std::string>& fingerprintsUrls);
};

