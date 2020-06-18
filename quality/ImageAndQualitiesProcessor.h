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
	FileWrapper files;
	SecugenWrapper secugen; 
	static std::mutex ReadLock;
	static std::mutex WriteLock;
	std::vector<std::string>& _fingerprintsUrls;
	static std::atomic<unsigned int> counter;
	std::string _url;
	unsigned int _quality;

public:
	std::string FetchImageUrl();
	void ProcessImage();
	void UploadResults();
	ImageAndQualitiesProcessor(std::vector<std::string>& fingerprintsUrls);
};

