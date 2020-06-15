#pragma once
#include "NameUtilities.h"
#include "Image.h"
#include "FileWrapper.h"
#include "SecugenWrapper.h"
#include <mutex>

class ImageAndQualitiesProcessor
{
private:
	NameUtilities name;
	Image image;
	FileWrapper files;
	SecugenWrapper secugen; 
	std::mutex ReadLock;
	std::mutex WriteLock;
	std::vector<std::string>* _fingerprintsUrls;
	unsigned int _numberOfFingerprints;

public:
	std::string FetchImageUrl();
	unsigned int ProcessImage(std::string url);
	void UploadResults(std::string url, unsigned int quality);
	ImageAndQualitiesProcessor(std::vector<std::string>* fingerprintsUrls, unsigned int numberOfFingerprints, volatile unsigned int& count);

protected:
	volatile unsigned int& _count;
};

