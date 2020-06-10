#include <iostream>
#include <assert.h>
#include "SecugenWrapper.h"
#include "FileWrapper.h"
#include "GSUtilWrapper.h"

int main()
{
	FileWrapper files;
	SecugenWrapper secugen;
	assert(secugen.GetQuality(files.readFile("qualityscore19.raw").data()) == 19);
	assert(secugen.GetQuality(files.readFile("qualityscore59.raw").data()) == 59);
	assert(secugen.GetQuality(files.readFile("qualityscore99.raw").data()) == 99);

	GSUtilWrapper gsutil;
	gsutil.SetBucket("simprints-152315-images-eu");
	gsutil.SetProject("fUBnpzDdbsCsMp0egCHB");
	gsutil.ListFingerprintImages({});
	//system("gsutil");
	//system("gsutil >0 cp \"gs://simprints-152315-images-eu/projects/fUBnpzDdbsCsMp0egCHB/sessions/78ee802e-a352-49e6-bc10-1e221054250a/fingerprints/6797b090-c4a5-4480-b407-7515d71ece19.wsq\" .");
	//system("gsutil cp \"gs://simprints-152315-images-eu/projects/fUBnpzDdbsCsMp0egCHB/sessions/78ee802e-a352-49e6-bc10-1e221054250a/fingerprints/6797b090-c4a5-4480-b407-7515d71ece19.wsq\" . >0");
}