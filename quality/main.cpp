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

const std::string downloadFolder("images");
const std::string fingerprintsUrlsFilename("fingerprintslist.txt");
const std::string fingerprintQualitiesFilename("fingerprintQualities.csv");

void Test_SecugenFingerprintQuality() {
	FileWrapper files;
	SecugenWrapper secugen;
	assert(secugen.GetQuality(files.getBinary("qualityscore19.raw").data()) == 19);
	assert(secugen.GetQuality(files.getBinary("qualityscore59.raw").data()) == 59);
	assert(secugen.GetQuality(files.getBinary("qualityscore99.raw").data()) == 99);
}

void Stage1_CollectFingerprintImages() {
	// Stage 1: get fingerprint urls into txt file
	FileWrapper files;

	GsutilWrapper gsutil;
	gsutil.SetBucket("simprints-152315-images-eu");
	gsutil.SetProject("fUBnpzDdbsCsMp0egCHB");

	std::string urls = gsutil.ListFingerprintImages();
	files.writeFile(fingerprintsUrlsFilename.c_str(), urls);

	gsutil.DownloadAllWsq("images");
}

void Test_WsqImageQuality() {
	FileWrapper files;
	Image image;
	std::string outfile;
	image.DecodeWsqFile("test_images/1700dpi.wsq", &outfile);
	std::cout << "output: " << outfile << std::endl;
	std::vector<unsigned char> downsizedImage;
	image.Downsize(files.getBinary(outfile.c_str()), downsizedImage);

	SecugenWrapper secugen;
	secugen.GetQuality(downsizedImage.data());
}

void Test_ReadLists() 
{
	FileWrapper files;
	std::vector<std::string> vecOfStr;

	// Get the contents of file in a vector
	bool result = files.getLines("fingerprintslist.txt", vecOfStr);

	if (result)
	{
		// Print the vector contents
		for (std::string& line : vecOfStr)
			std::cout << line << std::endl;
	}
}

void Test_Download() 
{
	GsutilWrapper gsutil;
	std::string url("gs://simprints-152315-images-eu/projects/fUBnpzDdbsCsMp0egCHB/sessions/004e8db6-b294-487b-8334-6c8def57e54c/fingerprints/22804a1d-0da0-4604-92b6-1042c455b8d2.wsq");
	std::string filename;
	NameUtilities name;
	name.getFilenameFromUrl(url, &filename);
	std::string destination = downloadFolder + "/" + filename;

	gsutil.Download(url, destination);
}

int Test_DownloadAndGetQuality() {
	FileWrapper files;
	Image image;
	GsutilWrapper gsutil;
	SecugenWrapper secugen;
	NameUtilities name;
	
	std::vector<std::string> vecOfStr;
	bool result = files.getLines("fingerprintslist.txt", vecOfStr);
	if (!result) {
		std::cout << "Error: couldnt read lines" << std::endl;
		return 1;
	}

	for (std::string& line : vecOfStr) {

		std::string url(line);
		std::string filename;
		name.getFilenameFromUrl(url, &filename);
		std::string destination = downloadFolder + "/" + filename;
		gsutil.Download(url, destination);


		std::string outfile;
		image.DecodeWsqFile(destination, &outfile);
		std::cout << "output: " << outfile << std::endl;
		std::vector<unsigned char> downsizedImage;
		image.Downsize(files.getBinary(outfile.c_str()), downsizedImage);

		secugen.GetQuality(downsizedImage.data());

	}
	return 0;
}

void Test_WriteCsv() 
{
	std::vector<std::pair<std::string, unsigned int>> imagesAndQualities;

	imagesAndQualities.push_back(std::make_pair("first", 10));
	imagesAndQualities.push_back(std::make_pair("second", 20));
	FileWrapper files;
	files.writePairsFile("pairs.csv", imagesAndQualities);
}

std::vector<std::string> fingerprintsUrls;
unsigned int numberOfFingerprints;
std::mutex ReadLock;
std::mutex WriteLock;

SecugenWrapper secugen; //TODO does this need to be initialised?

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
		//std::cout << id << " is fetching " << filename << std::endl;

		count++;
		std::cout << count << "/" << numberOfFingerprints << std::endl;
	}
	return url;
}

unsigned int ProcessImage(std::string url) {
	NameUtilities name;
	std::string filename;
	name.getFilenameFromUrl(url, &filename);
	//std::cout << "Processing: " << filename << std::endl;

	Image image;
	FileWrapper files;
	std::string wsqfilename;
	name.getFilenameFromUrl(url, &wsqfilename);
	std::string wsqdestination = downloadFolder + "/" + wsqfilename;

	std::string rawfilename;
	image.DecodeWsqFile(wsqdestination, &rawfilename);
	//std::cout << "output: " << rawfilename << std::endl;
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

const unsigned int NUM_THREADS = 5;
void Stage2_RunQuality() {
	FileWrapper files;
	bool result = files.getLines(fingerprintsUrlsFilename, fingerprintsUrls);
	if (!result) {
		std::cout << "Error: couldnt read lines" << std::endl;
		return;
	}
	numberOfFingerprints = fingerprintsUrls.size();
	files.writeFile(fingerprintQualitiesFilename.c_str(), "");

	std::thread threads[NUM_THREADS];
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

	if (fingerprintQualities.size() == fingerprintsUrls.size()) {
		std::cout << "Success: Number of output fingerprints = number of input fingerprints = " << fingerprintQualities.size() << std::endl;
		std::cout << "Please now delete the contents of the folder: images/" << std::endl;
	}
	else {
		std::cout << "Error: Number of output fingerprints != number of input fingerprints" << std::endl;
		std::cout << "Number of output fingerprints = " << fingerprintQualities.size() << std::endl;
		std::cout << "Number of input fingerprints = " << fingerprintsUrls.size() << std::endl;
	}
}

int main()
{
	//TODO
	//add final check that number of files are same
	//	delete fingerprints


	//Stage 1: Download all WSQ images
	//Stage1_CollectFingerprintImages();

	FileWrapper files;
	bool result = files.getLines(fingerprintsUrlsFilename, fingerprintsUrls);
	if (!result) {
		std::cout << "Error: couldnt read lines" << std::endl;
		return 1;
	}

	//Stage 2: Run it through quality SDK via parallel processing
	//Stage2_RunQuality();

	//Stage 3: Wrap up: confirm number, delete images
	Stage3_Confirm();

	return 0;
}