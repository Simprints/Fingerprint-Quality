#include "Image.h"
#include "SystemWrapper.h"
#include <iostream>
#include <cmath>

#define cimg_display 0
#include "lib/CImg/CImg.h"
using namespace cimg_library;

const std::string DWSQ("C:\\dev\\Fingerprint-Quality\\quality\\lib\\wsq\\dwsq.exe");

std::string Image::Decode() {
	std::string filetype = GetFileType();

	if (filetype == "wsq") {
		DecodeWsqFile(_path, &_decodedFilePath);
	}

	if (filetype == "raw") {
		_decodedFilePath = _path;
	}

	if (filetype == "pgm") {
		std::cout << "Error: unsupported pgm decoding" << std::endl;
	}

	return _decodedFilePath;

}

void Image::DecodeWsqFile(std::string filename, std::string* filename_out)
{
	SystemWrapper system;
	std::cout << system.run(DWSQ + " raw " + filename + " -raw_out");
	*filename_out = filename.substr(0, filename.size() - 3) + "raw";

}

unsigned int CalculateWidth(const unsigned int area) {
	//assuming height = 4/3 * width
	return std::sqrt(area * 3 / 4);
}

unsigned int CalculateHeight(const unsigned int width) {
	//assuming height = 4/3 * width
	return width * 4/3;
}

void Image::Downsize(const std::vector<unsigned char> input, std::vector<unsigned char>& output)
{
	unsigned int width = CalculateWidth(input.size());
	unsigned int height = CalculateHeight(width);
	//std::cout << "width=" << width << " height=" << height << std::endl;
    CImg<uint8_t> image(input.data(), width, height);

    CImg<uint8_t> downsizedImage = image.get_resize(300, 400);
	output.insert(output.begin(), downsizedImage.data(), downsizedImage.data() + downsizedImage.size());

}

std::string Image::getFilename() {
	std::size_t found = _path.find_last_of("/\\");
	return _path.substr(found + 1);
}

std::string Image::GetFileType() {
	std::size_t found = _path.find_last_of(".");
	return _path.substr(found + 1);
}

Image::Image(std::string path): _path(path)
{

}
