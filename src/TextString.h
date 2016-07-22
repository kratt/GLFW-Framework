// 22.07.2016
// author: Julian Kratt

#ifndef TEXT_STRING_H
#define TEXT_STRING_H

#include <string> 
#include <iostream>
#include "VertexBufferObjectAttribs.h"


struct TextString 
{
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

#endif
