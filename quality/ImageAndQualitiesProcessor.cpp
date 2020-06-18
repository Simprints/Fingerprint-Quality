#include "ImageAndQualitiesProcessor.h"

#include "constants.h"
#include <iostream>

std::mutex ImageAndQualitiesProcessor::ReadLock;
std::mutex ImageAndQualitiesProcessor::WriteLock;
std::atomic<unsigned int> ImageAndQualitiesProcessor::counter(0);
extern unsigned int numberOfFingerprints;

std::string ImageAndQualitiesProcessor::FetchImageUrl() {
	std::lock_guard<std::mutex> lock(ReadLock);

	if (_fingerprintsUrls.empty()) {
		_url = "";
	}
	else {
		_url = _fingerprintsUrls.back();
		_fingerprintsUrls.pop_back();

		counter++;
		std::cout << counter << "/" << numberOfFingerprints << std::endl;
	}

	return _url;
}

std::string GetLocalPath(std::string url) {
	std::string filename = getFilename(url);
	std::string path = imagesFolder + "/" + filename;
	return path;
}

void ImageAndQualitiesProcessor::ProcessImage() {	
	std::string path = FromGsutil ? GetLocalPath(_url) : _url;
	
	Image image(path);
	std::string decodedFilename = image.Decode();
	std::vector<unsigned char> downsizedImage;
	image.Downsize(files.getBinary(decodedFilename.c_str()), downsizedImage);

	_quality = secugen.GetQuality(downsizedImage.data());
}

void ImageAndQualitiesProcessor::UploadResults() {
	std::lock_guard<std::mutex> lock(WriteLock);
	
	FileWrapper files;
	files.appendToFile(fingerprintQualitiesFilename.c_str(), std::make_pair(_url, _quality));
}

ImageAndQualitiesProcessor::ImageAndQualitiesProcessor(std::vector<std::string>& fingerprintsUrls):
	_fingerprintsUrls(fingerprintsUrls), _quality(0), _url("")
{
}
