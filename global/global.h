#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <string>
#include <vector>

/**
 * @brief Write 8bit image to a ppm file.
 * @param filename
 * @param image_width
 * @param image_height
 * @param image_data
*/
void WriteToPPM(std::string filename, unsigned int image_width, unsigned int image_height, const unsigned char* image_data);


int StringToInt(std::string str);
double StringToDouble(std::string str);



#endif//__GLOBAL_H__