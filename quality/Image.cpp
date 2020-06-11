#include "Image.h"
extern "C" {
#include "lib/wsq/include/wsq.h"
}

#define cimg_display 0
#include "lib/CImg/CImg.h"
using namespace cimg_library;

void Image::DecodeWsq(const unsigned char* input, unsigned char** output)
{
}

//void Image::Downsize(const std::vector<BYTE>& input, std::vector<BYTE>& output)
//{
//}
