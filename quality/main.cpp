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
#include "DirectoryWrapper.h"

std::vector<std::string> fingerprintsUrls;
unsigned int numberOfFingerprints;



void ListKenya2019Fingerprints() {

}

void DownloadAndListAfyaTekFingerprints() {
	FileWrapper files;

	GsutilWrapper gsutil;
	gsutil.SetBucket(gsutilBucket);
	gsutil.SetProject(gsutilProject);

	std::string urls = gsutil.ListFingerprintImages();
	files.writeFile(fingerprintsUrlsFilename.c_str(), urls);

	gsutil.DownloadAllWsq(imagesFolder);
}

void ExecuteThread() {
	ImageAndQualitiesProcessor proc(fingerprintsUrls);
	for (;;) {
		std::string url = proc.FetchImageUrl();
		if (url.empty()) {
			break;
		}

		proc.ProcessImage();

		proc.UploadResults();
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

bool Stage3_Confirm() {
	std::vector<std::string> fingerprintQualities;
	FileWrapper files;
	bool result = files.getLines(fingerprintQualitiesFilename, fingerprintQualities);
	if (!result) {
		std::cout << "Error: couldnt read lines" << std::endl;
		return false;
	}

	if (fingerprintQualities.size() == numberOfFingerprints) {
		std::cout << "Success: Number of output fingerprints = number of input fingerprints = " << fingerprintQualities.size() << std::endl;
		std::cout << "Please now delete the contents of the folder: " << imagesFolder << std::endl;
	}
	else {
		std::cout << "Error: Number of output fingerprints != number of input fingerprints" << std::endl;
		std::cout << "Number of output fingerprints = " << fingerprintQualities.size() << std::endl;
		std::cout << "Number of input fingerprints = " << numberOfFingerprints << std::endl;
		return false;
	}
	return true;
}

bool LoadFingerprintUrls() {
	FileWrapper files;
	bool result = files.getLines(fingerprintsUrlsFilename, fingerprintsUrls);
	if (!result) {
		std::cout << "Error: couldnt read lines" << std::endl;
		return false;
	}
	numberOfFingerprints = fingerprintsUrls.size();
	std::cout << "Found " << numberOfFingerprints << " fingerprints" << std::endl;
	return true;
}

void InitFingerprintQualitiesCsv() {
	FileWrapper files;
	files.writeFile(fingerprintQualitiesFilename.c_str(), "");
}

bool Stage1_LoadFingerprintImages() {
	DownloadAndListAfyaTekFingerprints();

	return LoadFingerprintUrls();
}

int main()
{
	DirectoryWrapper dir;
	std::cout << dir.ListFingerprintImages("Kenya2019");
	//int startTime = clock();
	//InitFingerprintQualitiesCsv();

	//if (!Stage1_LoadFingerprintImages()) {
	//	return 0;
	//}	

	//Stage2_RunQuality();

	//Stage3_Confirm();

	//int endTime = clock();
	//std::cout << "Elapsed time: " << (endTime - startTime) / double(CLOCKS_PER_SEC) << std::endl;

	return 0;
}