// 20.07.2016
// author: Julian Kratt
// based on: https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Text_Rendering_02

#ifndef TEXTUR_ATLAS_H
#define TEXTUR_ATLAS_H

#include "GlyphMetrics.h"
#include "Global.h"
#include <ft2build.h>
#include FT_FREETYPE_H

#include <algorithm>
#include <iostream>


class TextureAtlas
{
#define MAXWIDTH 1024

public:
	TextureAtlas(const std::string &font, unsigned int fontSize);
	~TextureAtlas();

	GlyphMetric metrics(unsigned int);
	unsigned int width() const;
	unsigned int height() const;
	unsigned int fontSize() const;

	std::string font() const;

	GLuint texId() const;

private:
	void init();

private:
	unsigned int m_fontSize;
	unsigned int m_width;
	unsigned int m_height;

	std::string m_font;
	GLuint m_texId;

	GlyphMetric c[128];
};

#endif
