#include <iostream>
#include <assert.h>
#include "SecugenWrapper.h"
#include "FileWrapper.h"
#include "GSUtilWrapper.h"
#include "Image.h"
#include "SystemWrapper.h"

void Test_SecugenFingerprintQuality() {
	FileWrapper files;
	SecugenWrapper secugen;
	assert(secugen.GetQuality(files.readFile("qualityscore19.raw").data()) == 19);
	assert(secugen.GetQuality(files.readFile("qualityscore59.raw").data()) == 59);
	assert(secugen.GetQuality(files.readFile("qualityscore99.raw").data()) == 99);
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


int main()
{
	//Test_SecugenFingerprintQuality();
	//Stage1_CollectFingerprintLists();

	//stage 2:
	FileWrapper files;
	Image image;
	std::string outfile;
	image.DecodeWsqFile("images/1700dpi.wsq", &outfile);
	std::cout << "output: " << outfile << std::endl;
	std::vector<unsigned char> downsizedImage;
	image.Downsize(files.readFile(outfile.c_str()), downsizedImage);

	SecugenWrapper secugen;
	secugen.GetQuality(downsizedImage.data());

	// Stage 2: read txt file line by line and download fingerprint, get quality, save to csv file {url, quality} 

}