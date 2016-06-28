#ifndef TEXT_STRING_H
#define TEXT_STRING_H

//https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Text_Rendering_01
//https://gitlab.com/wikibooks-opengl/modern-tutorials/blob/master/text01_intro/text.cpp
//https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Text_Rendering_02

#include "Global.h"

#include <string> 
#include <glm/vec2.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H


class Shader;
class VertexBufferObjectAttribs;
class Texture;


class TextString
{
public: 
	TextString(std::string text, glm::vec2 screenPos);
	~TextString();

	GLuint texId() const;
	glm::vec2 dims() const;
	glm::vec2 pos() const;


private:
	void initTexture();
	void initTextureSdf();

	void distanceField(FT_Bitmap* bitmap, unsigned char** outData);
	void unpackMonoBitmap(FT_Bitmap* bitmap, unsigned char** outData);
	
private:
	std::string m_text;

	GLuint m_texId;
	glm::vec2 m_pos;
	glm::vec2 m_dims;
};

#endif
