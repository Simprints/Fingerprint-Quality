#include <iostream>
#include <assert.h>
#include "SecugenWrapper.h"
#include "FileWrapper.h"
#include "GSUtilWrapper.h"
extern "C" {
#include "lib/wsq/include/wsq.h"
}

#define cimg_display 0
#include "lib/CImg/CImg.h"
using namespace cimg_library;

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
	Test_SecugenFingerprintQuality();
	//Stage1_CollectFingerprintLists();

	// Stage 2: read txt file line by line and download fingerprint, get quality, save to csv file {url, quality} 

}