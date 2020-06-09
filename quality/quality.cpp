#include <iostream>
#include <assert.h>
#include "SecugenWrapper.h"
#include "FileWrapper.h"

int main()
{
	FileWrapper files;
	SecugenWrapper secugen;
	assert(secugen.GetQuality(files.readFile("qualityscore19.raw").data()) == 19);
	assert(secugen.GetQuality(files.readFile("qualityscore59.raw").data()) == 59);
	assert(secugen.GetQuality(files.readFile("qualityscore99.raw").data()) == 99);

}