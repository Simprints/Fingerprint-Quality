#pragma once
#include <Windows.h>
//#include "sgfplib.h"
#include "lib/sgfplib/sgfplib.h"
class SecugenWrapper {

public:
	int GetQuality(BYTE* fingerprintImage);
	SecugenWrapper();

private:
	HSGFPM hFPM;

};

