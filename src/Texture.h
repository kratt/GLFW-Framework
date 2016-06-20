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
	GLuint m_texId;
	
	unsigned int m_width;
	unsigned int m_height;

	std::string m_fileName;
};

#endif