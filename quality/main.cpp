#include <iostream>
#include <assert.h>
#include "SecugenWrapper.h"
#include "FileWrapper.h"
#include "GSUtilWrapper.h"
#include "Image.h"
#include "SystemWrapper.h"
#include <fstream>
#include <string>
#include "NameUtilities.h"
#include <thread>
#include <mutex>
#include "constants.h"
#include <time.h>

const unsigned int NUM_THREADS = 5;
std::vector<std::string> fingerprintsUrls;
unsigned int numberOfFingerprints;
std::mutex ReadLock;
std::mutex WriteLock;

SecugenWrapper secugen; //TODO does this need to be initialised?

void Stage1_CollectFingerprintImages() {
	// Stage 1: get fingerprint urls into txt file
	FileWrapper files;

	GsutilWrapper gsutil;
	gsutil.SetBucket(gsutilBucket);
	gsutil.SetProject(gsutilProject);

	std::string urls = gsutil.ListFingerprintImages();
	files.writeFile(fingerprintsUrlsFilename.c_str(), urls);

	gsutil.DownloadAllWsq(downloadFolder);
}

std::string FetchImageUrl(int id) {
	std::lock_guard<std::mutex> lock(ReadLock);
	static unsigned int count = 0;
	NameUtilities name;	
	std::string url;
	if (fingerprintsUrls.empty()) {
		url = "";
	}
	else {
		url = fingerprintsUrls.back();
		fingerprintsUrls.pop_back();
		std::string filename;
		name.getFilenameFromUrl(url, &filename);

		count++;
		std::cout << count << "/" << numberOfFingerprints << std::endl;
	}
	return url;
}

unsigned int ProcessImage(std::string url) {
	NameUtilities name;
	Image image;
	FileWrapper files;
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

void UploadResults(std::string url, unsigned int quality) {
	std::lock_guard<std::mutex> lock(WriteLock);
	NameUtilities name;
	std::string filename;
	name.getFilenameFromUrl(url, &filename);
	//std::cout << "Uploading: " << filename << " as " << quality << std::endl;
	FileWrapper files;
	files.appendToFile(fingerprintQualitiesFilename.c_str(), std::make_pair(url, quality));
}

void ExecuteThread(int id) {
	for (;;) {
		std::string url = FetchImageUrl(id);
		if (url.empty()) {
			break;
		}

		unsigned int quality = ProcessImage(url);

		UploadResults(url, quality);
	}
}


void Stage2_RunQuality() {
	std::thread threads[NUM_THREADS];
	std::cout << "Starting " << NUM_THREADS << " threads" << std::endl;
	for (int i = 0; i < NUM_THREADS; i++) {
		threads[i] = std::thread(ExecuteThread, i);
	}

	for (int i = 0; i < NUM_THREADS; i++) {
		threads[i].join();
	}
}

void Stage3_Confirm() {
	std::vector<std::string> fingerprintQualities;
	FileWrapper files;
	bool result = files.getLines(fingerprintQualitiesFilename, fingerprintQualities);
	if (!result) {
		std::cout << "Error: couldnt read lines" << std::endl;
		return;
	}

	if (fingerprintQualities.size() == numberOfFingerprints) {
		std::cout << "Success: Number of output fingerprints = number of input fingerprints = " << fingerprintQualities.size() << std::endl;
		std::cout << "Please now delete the contents of the folder: " << downloadFolder << std::endl;
	}
	else {
		std::cout << "Error: Number of output fingerprints != number of input fingerprints" << std::endl;
		std::cout << "Number of output fingerprints = " << fingerprintQualities.size() << std::endl;
		std::cout << "Number of input fingerprints = " << numberOfFingerprints << std::endl;
	}
}

int main()
{
	int startTime = clock();
	//Stage 1: Download all WSQ images
	//Stage1_CollectFingerprintImages();

	FileWrapper files;
	bool result = files.getLines(fingerprintsUrlsFilename, fingerprintsUrls);
	if (!result) {
		std::cout << "Error: couldnt read lines" << std::endl;
		return 1;
	}
	numberOfFingerprints = fingerprintsUrls.size();
	std::cout << "Found " << numberOfFingerprints << " fingerprints" << std::endl;

	files.writeFile(fingerprintQualitiesFilename.c_str(), "");

	//Stage 2: Run it through quality SDK via parallel processing
	Stage2_RunQuality();

	//Stage 3: Wrap up: confirm number, delete images
	Stage3_Confirm();

	int endTime = clock();
	std::cout << "Elapsed time: " << (endTime - startTime) / double(CLOCKS_PER_SEC) << std::endl;

	return 0;
}