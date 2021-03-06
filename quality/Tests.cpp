#include "Tests.h"
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

void Test_SecugenFingerprintQuality() {
	FileWrapper files;
	SecugenWrapper secugen;
	assert(secugen.GetQuality(files.getBinary("qualityscore19.raw").data()) == 19);
	assert(secugen.GetQuality(files.getBinary("qualityscore59.raw").data()) == 59);
	assert(secugen.GetQuality(files.getBinary("qualityscore99.raw").data()) == 99);
}

void Test_WsqImageQuality() {
	FileWrapper files;
	Image image("test_images/1700dpi.wsq");
	std::string outfile;
	outfile = image.Decode();
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
	std::string filename = getFilename(url);
	std::string destination = imagesFolder + "/" + filename;

	gsutil.Download(url, destination);
}

int Test_DownloadAndGetQuality() {
	FileWrapper files;
	
	GsutilWrapper gsutil;
	SecugenWrapper secugen;

	std::vector<std::string> vecOfStr;
	bool result = files.getLines("fingerprintslist.txt", vecOfStr);
	if (!result) {
		std::cout << "Error: couldnt read lines" << std::endl;
		return 1;
	}

	for (std::string& line : vecOfStr) {

		std::string url(line);
		std::string filename = getFilename(url);
		std::string destination = imagesFolder + "/" + filename;
		gsutil.Download(url, destination);


		std::string outfile;
		Image image(destination);
		outfile = image.Decode();
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

void Test_Name()
{
	std::string url("gs://simprints-152315-images-eu/projects/fUBnpzDdbsCsMp0egCHB/sessions/004e8db6-b294-487b-8334-6c8def57e54c/fingerprints/22804a1d-0da0-4604-92b6-1042c455b8d2.wsq");
	std::string filename = getFilename(url);
	assert(filename == "22804a1d-0da0-4604-92b6-1042c455b8d2.wsq");
	std::cout << filename << std::endl;

	filename.clear();
	url = "22804a1d-0da0-4604-92b6-1042c455b8d2.wsq";
	filename = getFilename(url);
	assert(filename == "22804a1d-0da0-4604-92b6-1042c455b8d2.wsq");
	std::cout << filename << std::endl;

	filename.clear();
	url = "22804a1d-0da0-4604-92b6-1042c455b8d2.raw";
	filename = getFilename(url);
	assert(filename == "22804a1d-0da0-4604-92b6-1042c455b8d2.raw");
	std::cout << filename << std::endl;

	filename.clear();
	url = "fingerprints/22804a1d-0da0-4604-92b6-1042c455b8d2.raw";
	filename = getFilename(url);
	assert(filename == "22804a1d-0da0-4604-92b6-1042c455b8d2.raw");
	std::cout << filename << std::endl;

}

void TestMain() {
	Test_Name();
}