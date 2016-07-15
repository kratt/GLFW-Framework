#ifndef OPENGL_UTILS_H
#define OPENGL_UTILS_H

#include "Global.h"
#include <string>
#include <vector>


namespace utils 
{
	bool save_framebuffer(const std::string &path);
	bool save_depthbuffer(const std::string &path);

	std::vector<unsigned char> get_framebuffer_data(const GLint x, const GLint y, const GLsizei width, const GLsizei height);
	std::vector<float> get_depthbuffer_data(const GLint x, const GLint y, const GLsizei width, const GLsizei height);

}


#endif
