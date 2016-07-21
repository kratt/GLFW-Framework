#ifndef TEXT_STRING_H
#define TEXT_STRING_H


#include <string> 
#include <iostream>
#include "VertexBufferObjectAttribs.h"


struct TextString {
	std::string text;
	std::string font;

	unsigned int fontSize;
	float width;
	float height;

	VertexBufferObjectAttribs *vbo;

	TextString()
	:	fontSize(0),
		width(0.0f),
		height(0.0f),
		vbo(nullptr) {};

	~TextString() {
		delete vbo;
	}
};





//class TextString
//{
//public: 
//	TextString(std::string text, int fontSize = 24, const std::string &font = "calibril");
//	~TextString();
//
//	GLuint texId() const;
//	glm::vec2 dims() const;
//	float offsetBaseline() const;
//
//	int fontSize() const;
//	std::string font() const;
//	std::string text() const;
//
//private:
//	void initTexture();
//	void initTextureSdf();
//
//private:
//	std::string m_text;
//	std::string m_font;
//	std::string m_fontPath;
//
//	int m_fontSize;
//
//	GLuint m_texId;
//	glm::vec2 m_dims; 
//	float m_offsetBaseline;
//};

#endif
