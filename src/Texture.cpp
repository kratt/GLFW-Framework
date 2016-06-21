#include "Texture.h"
#include "PngLoader.h"
#include <iostream>

Texture::Texture()
	: m_id(0)
{
}

Texture::Texture(const std::string fileName)
: m_fileName(fileName),
  m_id(0),
	m_target(GL_TEXTURE_2D),
	m_mipLevel(0),
	m_internalFormat(GL_RGBA),
	m_format(GL_RGB),
	m_border(0),
	m_type(GL_UNSIGNED_BYTE),
	m_minFilter(GL_LINEAR),
	m_magFilter(GL_LINEAR),
	m_wrap(GL_REPEAT),
	m_createMipMaps(GL_TRUE),
	m_maxAnisotropy(16.0f),
	m_manualMipMaps(GL_FALSE)
{
	createTexture(fileName);
}

void Texture::load(const std::string fileName)
{
	m_fileName = fileName;
	createTexture(fileName);}


GLuint Texture::id() const
{
	return m_id;
}

void Texture::createTexture(const std::string fileName)
{
	PngLoader png;// (fileName);

	GLubyte *textureImage;
	int width, height;
	bool hasAlpha;
	bool success = png.loadPngImage(fileName.c_str(), width, height, hasAlpha, &textureImage);
	if (!success) {
		std::cout << "Unable to load png file" << std::endl;
		return;
	}

	std::cout << "width: " << width << " " << "heiht: " << height << std::endl;

	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			int idx = 4 * (y*width + x);
			std::cout << int(textureImage[idx]) << " " << int(textureImage[idx + 1]) << " " << int(textureImage[idx + 2]) << " " << int(textureImage[idx + 3]) << std::endl;
		}
	}






	//m_width = png.width();
	//m_height = png.height();

	//unsigned char *imageData = png.imageData();

	//glGenTextures(1, &m_id);
	//glBindTexture(GL_TEXTURE_2D, m_texId);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)imageData);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glBindTexture(GL_TEXTURE_2D, 0);

	//bool hasAlpha = png.hasAlpha();

	GLenum format = hasAlpha ? GL_RGBA8 : GL_RGB8;
	GLenum internalFormat = hasAlpha ? GL_RGBA : GL_RGB;

	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);
	glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, hasAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, textureImage);

	//glTexImage2D(GL_TEXTURE_2D, 0, hasAlpha ? 4 : 3, width, height, 0, hasAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, textureImage);
	//glTexImage2D(m_target, 4, GL_RGBA8, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)imageData);
	glGenerateMipmap(GL_TEXTURE_2D);  //Generate num_mipmaps number of mipmaps here.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);


	//if (m_id)
	//	glDeleteTextures(1, &m_id);

	//glGenTextures(1, &m_id);
	//glBindTexture(m_target, m_id);

	//if (m_createMipMaps)
	//{
	//	if (m_manualMipMaps)
	//	{
	//		/*		for (uint i = 0; i<m_manualMM.size(); ++i)
	//				{
	//					QImage img = m_manualMM[i];
	//					glTexImage2D(m_target, i, m_internalFormat, img.width(), img.height(), m_border, m_format, m_type, img.bits());
	//				}

	//					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, m_manualMM.size() - 1);*/
	//	}
	//	else
	//	{
	//		glGenerateMipmap(m_target);
	//		glTexImage2D(m_target, m_mipLevel, m_internalFormat, m_width, m_height, m_border, m_format, m_type, (GLvoid*)imageData);
	//		
	//	}
	//}
	//else
	//{
	//	glTexImage2D(m_target, 0, m_internalFormat, m_width, m_height, m_border, m_format, m_type, (GLvoid*)imageData);
	//}


	//glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, m_minFilter);
	//glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, m_magFilter);

	//glTexParameterf(m_target, GL_TEXTURE_WRAP_S, m_wrap);
	//glTexParameterf(m_target, GL_TEXTURE_WRAP_T, m_wrap);
	//glTexParameterf(m_target, GL_TEXTURE_WRAP_R, m_wrap);

	//glTexParameterf(m_target, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_maxAnisotropy);

	//glBindTexture(m_target, 0);

	//png.clear();
}


