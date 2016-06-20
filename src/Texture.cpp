#include "Texture.h"
#include "PngLoader.h"

Texture::Texture()
{
}

Texture::Texture(const std::string fileName)
: m_fileName(fileName)
{
	createTexture(fileName);
}

void Texture::load(const std::string fileName)
{
	m_fileName = fileName;
	createTexture(fileName);}


GLuint Texture::id() const
{
	return m_texId;
}

void Texture::createTexture(const std::string fileName)
{
	PngLoader png(fileName);

	int width = png.width();
	int height = png.height();

	unsigned char *imageData = png.imageData();

	glGenTextures(1, &m_texId);
	glBindTexture(GL_TEXTURE_2D, m_texId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)imageData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
}


