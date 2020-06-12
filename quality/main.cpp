#include <iostream>
#include <assert.h>
#include "SecugenWrapper.h"
#include "FileWrapper.h"
#include "GSUtilWrapper.h"
#include "Image.h"
#include "SystemWrapper.h"
#include <fstream>
#include <string>

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

void Stage1_CollectFingerprintLists() {
	// Stage 1: get fingerprint urls into txt file
	FileWrapper files;

	GsutilWrapper gsutil;
	gsutil.SetBucket("simprints-152315-images-eu");
	gsutil.SetProject("fUBnpzDdbsCsMp0egCHB");

	std::string urls = gsutil.ListFingerprintImages();
	files.writeFile(fingerprintsUrlsFilename.c_str(), urls);
}

void Test_WsqImageQuality() {
	FileWrapper files;
	Image image;
	std::string outfile;
	image.DecodeWsqFile("images/1700dpi.wsq", &outfile);
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
	gsutil.getFilenameFromUrl(url, &filename);
	std::string destination = downloadFolder + "/" + filename;

	gsutil.Download(url, destination);
}

int Test_DownloadAndGetQuality() {
	FileWrapper files;
	Image image;
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
		std::string filename;
		gsutil.getFilenameFromUrl(url, &filename);
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

int main()
{

	//Test_SecugenFingerprintQuality();
	//Stage1_CollectFingerprintLists();
	//Test_WsqImageQuality();
	//Test_ReadLists();
	//Test_Download();
	//Test_DownloadAndGetQuality();
	//Test_WriteCsv();

	FileWrapper files;
	Image image;
	GsutilWrapper gsutil;
	SecugenWrapper secugen;
	std::vector<std::string> fingerprintsUrls;
	bool result = files.getLines(fingerprintsUrlsFilename, fingerprintsUrls);
	if (!result) {
		std::cout << "Error: couldnt read lines" << std::endl;
		return 1;
	}

	files.writeFile(fingerprintQualitiesFilename.c_str(), "");

	for (std::string& url : fingerprintsUrls) 
	{
		std::string wsqFilename;
		gsutil.getFilenameFromUrl(url, &wsqFilename);
		std::string wsqDestination = downloadFolder + "/" + wsqFilename;
		gsutil.Download(url, wsqDestination);

		std::string rawFilename;
		image.DecodeWsqFile(wsqDestination, &rawFilename);
		std::cout << "output: " << rawFilename << std::endl;
		std::vector<unsigned char> downsizedImage;
		image.Downsize(files.getBinary(rawFilename.c_str()), downsizedImage);

		unsigned int quality = secugen.GetQuality(downsizedImage.data());

		files.appendToFile(fingerprintQualitiesFilename.c_str(), std::make_pair(url, quality));
	}
	return 0;

}