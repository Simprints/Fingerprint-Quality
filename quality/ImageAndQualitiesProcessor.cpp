#include "ImageAndQualitiesProcessor.h"

#include "constants.h"
#include <iostream>

std::mutex ImageAndQualitiesProcessor::ReadLock;
std::mutex ImageAndQualitiesProcessor::WriteLock;
std::atomic<unsigned int> ImageAndQualitiesProcessor::counter(0);
extern unsigned int numberOfFingerprints;

std::string ImageAndQualitiesProcessor::FetchImageUrl() {
	std::lock_guard<std::mutex> lock(ReadLock);

	std::string url;
	if (_fingerprintsUrls.empty()) {
		url = "";
	}
	else {
		url = _fingerprintsUrls.back();
		_fingerprintsUrls.pop_back();

		counter++;
		std::cout << counter << "/" << numberOfFingerprints << std::endl;
	}
	_url = url;
	return url;
}

unsigned int ImageAndQualitiesProcessor::ProcessImage() {	

	std::string filename;
	name.getFilenameFromUrl(_url, &filename);
	std::string path = imagesFolder + "/" + filename;
	
	Image image(path);
	std::string decodedFilename = image.Decode();
	std::vector<unsigned char> downsizedImage;
	image.Downsize(files.getBinary(decodedFilename.c_str()), downsizedImage);

	unsigned int quality = secugen.GetQuality(downsizedImage.data());

	return quality;
}

void ImageAndQualitiesProcessor::UploadResults(unsigned int quality) {
	std::lock_guard<std::mutex> lock(WriteLock);

	std::string filename;
	name.getFilenameFromUrl(_url, &filename);
	//std::cout << "Uploading: " << filename << " as " << quality << std::endl;
	FileWrapper files;
	files.appendToFile(fingerprintQualitiesFilename.c_str(), std::make_pair(_url, quality));
}

ImageAndQualitiesProcessor::ImageAndQualitiesProcessor(std::vector<std::string>& fingerprintsUrls):
	_fingerprintsUrls(fingerprintsUrls)
{
}
