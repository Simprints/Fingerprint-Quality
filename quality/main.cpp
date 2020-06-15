#include <iostream>
#include <assert.h>
#include "FileWrapper.h"
#include "GSUtilWrapper.h"
#include <string>
#include "NameUtilities.h"
#include <thread>
#include "constants.h"
#include <time.h>
#include "Tests.h"
#include "ImageAndQualitiesProcessor.h"

std::vector<std::string> fingerprintsUrls;
unsigned int numberOfFingerprints;
volatile unsigned int count = 0;


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

void ExecuteThread() {
	ImageAndQualitiesProcessor proc(fingerprintsUrls, numberOfFingerprints);
	for (;;) {
		std::string url = proc.FetchImageUrl();
		if (url.empty()) {
			break;
		}

		unsigned int quality = proc.ProcessImage(url);

		proc.UploadResults(url, quality);
	}
}

void Stage2_RunQuality() {
	std::thread threads[NUM_THREADS];
	std::cout << "Starting " << NUM_THREADS << " threads" << std::endl;
	for (int i = 0; i < NUM_THREADS; i++) {
		threads[i] = std::thread(ExecuteThread);
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