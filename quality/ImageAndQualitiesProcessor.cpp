#include "ImageAndQualitiesProcessor.h"

#include "constants.h"
#include <iostream>

std::string ImageAndQualitiesProcessor::FetchImageUrl() {
	std::lock_guard<std::mutex> lock(ReadLock);
	//static unsigned int count = 0;

	std::string url;
	if (_fingerprintsUrls->empty()) {
		url = "";
	}
	else {
		url = _fingerprintsUrls->back();
		_fingerprintsUrls->pop_back();
		std::string filename;
		name.getFilenameFromUrl(url, &filename);

		_count++;
		std::cout << _count << "/" << _numberOfFingerprints << std::endl;
	}
	return url;
}

unsigned int ImageAndQualitiesProcessor::ProcessImage(std::string url) {


	std::string wsqfilename;
	name.getFilenameFromUrl(url, &wsqfilename);
	std::string wsqdestination = downloadFolder + "/" + wsqfilename;

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

ImageAndQualitiesProcessor::ImageAndQualitiesProcessor(std::vector<std::string>* fingerprintsUrls, unsigned int numberOfFingerprints, volatile unsigned int& count):
	_fingerprintsUrls(fingerprintsUrls), 
	_numberOfFingerprints(numberOfFingerprints),
	_count(count)
{
}
