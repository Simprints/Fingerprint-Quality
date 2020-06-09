#include <iostream>
#include <Windows.h>
#include "sgfplib.h"
#include <fstream>
#include <assert.h>
#include <vector>

class SecugenWrapper {

public:
	int GetQualityOfRaw(const char* filename);
	SecugenWrapper();

private:
	HSGFPM hFPM;

};

SecugenWrapper::SecugenWrapper() {
	DWORD rc = 0;
	rc = SGFPM_Create(&hFPM);
	if (rc != SGFDX_ERROR_NONE) {
		std::cout << "ERR: SGFPM_Create returns " << rc << std::endl;
	}
	std::cout << "Init" << std::endl;
}

std::vector<BYTE> readFile(const char* filename)
{
	std::ifstream file(filename, std::ios::binary);
	if (!file.is_open()) {
		std::cout << "Cannot open file" << std::endl;
		return {};
	}

	// Stop eating new lines in binary mode
	file.unsetf(std::ios::skipws);

	std::streampos fileSize;

	file.seekg(0, std::ios::end);
	fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<BYTE> vec;
	vec.reserve(fileSize);

	vec.insert(vec.begin(),
		std::istream_iterator<BYTE>(file),
		std::istream_iterator<BYTE>());

	return vec;
}

int SecugenWrapper::GetQualityOfRaw(const char* filename) {

	std::vector<BYTE> fingerprintImage = readFile(filename);

	DWORD img_quality;
	SGFPM_GetImageQuality(hFPM, 300, 400, fingerprintImage.data(), &img_quality);

	std::cout << "Quality:" << img_quality << std::endl;
	return img_quality;
}


int main()
{
	SecugenWrapper secugen;
	assert(secugen.GetQualityOfRaw("qualityscore19.raw") == 19);
	assert(secugen.GetQualityOfRaw("qualityscore59.raw") == 59);
	assert(secugen.GetQualityOfRaw("qualityscore99.raw") == 99);

}