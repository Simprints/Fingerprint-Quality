#include <iostream>
#include <Windows.h>
#include "sgfplib.h"
#include <fstream>
#include <assert.h>


int GetQualityOfFile(std::string filename) {
    DWORD rc = 0;
    HSGFPM hFPM = NULL;
    rc = SGFPM_Create(&hFPM);
    if (rc != SGFDX_ERROR_NONE) {
        std::cout << "ERR: SGFPM_Create returns " << rc << std::endl;
    }

    char buffer[120000];

    std::ifstream readFile(filename.c_str(), std::ios::out | std::ios::binary);

    //std::ifstream readFile("finger1_500.pgm", std::ios::out | std::ios::binary);
    //readFile.seekg(15, std::ios::beg);

    if (!readFile.read(buffer, 120000)) {
        std::cout << "Cannot open file" << std::endl;
        return 1;
    }

    BYTE* fingerprintImage = reinterpret_cast<BYTE*>(buffer);

    DWORD img_quality;
    SGFPM_GetImageQuality(hFPM, 300, 400, fingerprintImage, &img_quality);

    std::cout << "Quality:" << img_quality << std::endl;
    return img_quality;
}


int main()
{
    assert(GetQualityOfFile("qualityscore19.raw") == 19);
    assert(GetQualityOfFile("qualityscore59.raw") == 59);
    assert(GetQualityOfFile("qualityscore99.raw") == 99);

}