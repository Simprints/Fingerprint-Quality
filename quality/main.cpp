#include <iostream>
#include <assert.h>
#include "SecugenWrapper.h"
#include "FileWrapper.h"
#include "GSUtilWrapper.h"
#include "Image.h"
#include "SystemWrapper.h"
#include <fstream>


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
	files.writeFile("fingerprintslist.txt", urls);
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

int main()
{

	//Test_SecugenFingerprintQuality();
	//Stage1_CollectFingerprintLists();
	//Test_WsqImageQuality();
	//Test_ReadLists();

	// Stage 2: read txt file line by line and download fingerprint, get quality, save to csv file {url, quality} 

}