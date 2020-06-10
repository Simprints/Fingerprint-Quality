#include <iostream>
#include <assert.h>
#include "SecugenWrapper.h"
#include "FileWrapper.h"
#include "GSUtilWrapper.h"

#include <fstream>
#include <string>
#include <iostream>
void write()
{
	std::string input("hello");
	std::ofstream out("output.txt");
	out << input;
	out.close();

}

int main()
{
	FileWrapper files;
	SecugenWrapper secugen;
	assert(secugen.GetQuality(files.readFile("qualityscore19.raw").data()) == 19);
	assert(secugen.GetQuality(files.readFile("qualityscore59.raw").data()) == 59);
	assert(secugen.GetQuality(files.readFile("qualityscore99.raw").data()) == 99);

	GsutilWrapper gsutil;
	gsutil.SetBucket("simprints-152315-images-eu");
	gsutil.SetProject("fUBnpzDdbsCsMp0egCHB");
	
	std::string urls = gsutil.ListFingerprintImages();
	
	files.writeFile("fingerprintslist.txt", urls);

}