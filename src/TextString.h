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
	TextString(std::string text, int fontSize = 24, const std::string &font = "calibril");
	~TextString();

	GLuint texId() const;
	glm::vec2 dims() const;
	float offsetBaseline() const;

	int fontSize() const;
	std::string font() const;
	std::string text() const;

private:
	void initTexture();
	void initTextureSdf();

private:
	std::string m_text;
	std::string m_font;
	std::string m_fontPath;

	int m_fontSize;

	GLuint m_texId;
	glm::vec2 m_dims; 
	float m_offsetBaseline;
};

#endif
