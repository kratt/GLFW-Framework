#ifndef OPENGL_UTILS_H
#define OPENGL_UTILS_H

#include "Global.h"
#include <string>
#include <vector>


namespace utils 
{
	bool save_framebuffer(const std::string &path);
	bool save_depthbuffer(const std::string &path);
	bool save_texture(const std::string &path, GLuint id, bool saveMipMaps = false);
	bool save_texture_bind(const std::string &path, GLuint id, bool saveMipMaps = false);


	std::vector<unsigned char> get_framebuffer_data(const GLint x, const GLint y, const GLsizei width, const GLsizei height);
	std::vector<float> get_depthbuffer_data(const GLint x, const GLint y, const GLsizei width, const GLsizei height);

	GLint get_num_mipmaps(GLint texId);


	std::vector<GLuint> create_debug_textures();

}


#endif
