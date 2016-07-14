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

	bool save_image_rgb(const std::string &path, const std::vector<float> &data, unsigned int width, unsigned int height);
	bool save_image_rgba(const std::string &path, const std::vector<float> &data, unsigned int width, unsigned int height);
	bool save_image_alpha(const std::string &path, const std::vector<float> &data, unsigned int width, unsigned int height);

	bool save_image_rgb(const std::string &path, const std::vector<unsigned char> &data, unsigned int width, unsigned int height);
	bool save_image_rgba(const std::string &path, const std::vector<unsigned char> &data, unsigned int width, unsigned int height);
	bool save_image_alpha(const std::string &path, const std::vector<unsigned char> &data, unsigned int width, unsigned int height);


	bool save_framebuffer(const std::string &path);
}


#endif
