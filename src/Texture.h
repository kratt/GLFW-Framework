#ifndef TEXTURE_H
#define TEXTURE_H

#include "Global.h"

#include <string> 

class Texture
{

public:
	Texture();
	Texture(const std::string fileName);

	void load(const std::string fileName);
	GLuint id() const;

private: 
	void createTexture(const std::string fileName);

private:
	GLuint m_id;
	
	unsigned int m_width;
	unsigned int m_height;

	std::string m_fileName;

	GLenum  m_target;
	GLint   m_mipLevel;
	GLint   m_internalFormat;
	GLenum  m_format;
	GLint   m_border;
	GLenum  m_type;

	GLint   m_minFilter;
	GLint   m_magFilter;
	GLint   m_wrap;
	GLboolean m_createMipMaps;
	GLfloat m_maxAnisotropy;
	GLboolean m_manualMipMaps;
};

#endif