#ifndef IMAGE_UTILS_H
#define IMAGE_UTILS_H

#include "Global.h"
#include <string>
#include <vector>


namespace utils 
{
	bool load_image(const std::string &path, std::vector<unsigned char> &data,
                GLenum &type, unsigned int &width, unsigned int &height,
                unsigned int alignment);

	bool save_image(const std::string &path, std::vector<unsigned char> &data, unsigned int width, unsigned int height);
	int writeImage(char* filename, int width, int height, float *buffer, char* title);
}


#endif
