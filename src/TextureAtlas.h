// 20.07.2016
// author: Julian Kratt

#ifndef TEXTUR_ATLAS_H
#define TEXTUR_ATLAS_H

#include "Global.h"
#include <ft2build.h>
#include FT_FREETYPE_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <map>


class TextureAtlas
{
#define MAXWIDTH 1024
#define MAXGLYPHS 128

	struct{
		float ax;	// advance.x
		float ay;	// advance.y

		float bw;	// bitmap.width;
		float bh;	// bitmap.height;

		float bl;	// bitmap_left;
		float bt;	// bitmap_top;

		float tx;	// x offset of glyph in texture coordinates
		float ty;	// y offset of glyph in texture coordinates
	}c[128];

public:
	TextureAtlas(const std::string &font, unsigned int fontSize);
	~TextureAtlas();


private:
	void init();

private:
	unsigned int m_fontSize;
	unsigned int m_width;
	unsigned int m_height;

	std::string m_font;
	GLuint m_texId;
};

#endif
