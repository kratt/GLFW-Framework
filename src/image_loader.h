#ifndef UTILS_IMAGE_LOADER_H_
#define UTILS_IMAGE_LOADER_H_

#include "Global.h"
#include <string>
#include <vector>


namespace utils {
bool load_image(const std::string &path, std::vector<unsigned char> &data,
                GLenum &type, unsigned int &width, unsigned int &height,
                unsigned int alignment);
}


#endif
