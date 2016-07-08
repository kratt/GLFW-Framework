

#ifndef CGFW_UTILS_IMAGE_LOADER_H_
#define CGFW_UTILS_IMAGE_LOADER_H_

#include <cgfw/gl.h>
#include <string>
#include <vector>

namespace cgfw {
namespace utils {
bool load_image(const std::string &path, std::vector<unsigned char> &data,
                GLenum &type, unsigned int &width, unsigned int &height,
                unsigned int alignment);
}
}

#endif
