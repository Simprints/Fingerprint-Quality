#include "SecugenWrapper.h"
#include <iostream>
#include <vector>
SecugenWrapper::SecugenWrapper() {
	DWORD rc = 0;
	rc = SGFPM_Create(&hFPM);
	if (rc != SGFDX_ERROR_NONE) {
		std::cout << "ERR: SGFPM_Create returns " << rc << std::endl;
	}
	std::cout << "Init" << std::endl;
}

int SecugenWrapper::GetQuality(BYTE* fingerprintImage) {

	//std::vector<BYTE> fingerprintImage = readFile(filename);

	DWORD img_quality;
	SGFPM_GetImageQuality(hFPM, 300, 400, fingerprintImage, &img_quality);

	//std::cout << "Quality:" << img_quality << std::endl;
	return img_quality;
}