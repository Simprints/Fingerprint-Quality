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
		std::string filename;
		name.getFilenameFromUrl(url, &filename);

		counter++;
		std::cout << counter << "/" << numberOfFingerprints << std::endl;
	}
	return url;
}

unsigned int ImageAndQualitiesProcessor::ProcessImage(std::string url) {


	std::string wsqfilename;
	name.getFilenameFromUrl(url, &wsqfilename);
	std::string wsqdestination = imagesFolder + "/" + wsqfilename;

	std::string rawfilename;
	image.DecodeWsqFile(wsqdestination, &rawfilename);
	std::vector<unsigned char> downsizedimage;
	image.Downsize(files.getBinary(rawfilename.c_str()), downsizedimage);

	unsigned int quality = secugen.GetQuality(downsizedimage.data());

	return quality;
}

void ImageAndQualitiesProcessor::UploadResults(std::string url, unsigned int quality) {
	std::lock_guard<std::mutex> lock(WriteLock);

	std::string filename;
	name.getFilenameFromUrl(url, &filename);
	//std::cout << "Uploading: " << filename << " as " << quality << std::endl;
	FileWrapper files;
	files.appendToFile(fingerprintQualitiesFilename.c_str(), std::make_pair(url, quality));
}

ImageAndQualitiesProcessor::ImageAndQualitiesProcessor(std::vector<std::string>& fingerprintsUrls):
	_fingerprintsUrls(fingerprintsUrls)
{
}
