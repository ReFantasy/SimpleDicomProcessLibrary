#include "global.h"
#include <fstream>

#define PPMTYPE "P3"
#define MAXPIXELVALUE "255"


void WriteToPPM(std::string filename, unsigned int image_width, unsigned int image_height, const unsigned char* image_data)
{
	std::fstream io(filename, std::ios::out);

	io << PPMTYPE << "\n"
		<< image_width << char(0x20) << image_height
		<< "\n"
		<< MAXPIXELVALUE
		<< "\n";

	for (int j = 0; j < image_height; ++j)
	{
		for (int i = 0; i < image_width; ++i)
		{

			int ir = static_cast<int>(image_data[j * image_width + i]);
			int ig = static_cast<int>(image_data[j * image_width + i]);
			int ib = static_cast<int>(image_data[j * image_width + i]);

			io << ir << char(0x20) << ig << char(0x20) << ib << '\n';
		}
	}

	io.close();
}

int StringToInt(std::string str)
{
	return std::atoi(str.c_str());
}

double StringToDouble(std::string str)
{
	return std::atof(str.c_str());
}
