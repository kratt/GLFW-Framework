#include "TextureAtlas.h"

TextureAtlas::TextureAtlas(const std::string &font, unsigned int fontSize)
: m_font(font),
  m_fontSize(fontSize)
{
	init();
}

TextureAtlas::~TextureAtlas() {
	//glDeleteTextures(1, &tex);
}


void TextureAtlas::init()
{
	FT_Library ft;
	FT_Face face;

	if (FT_Init_FreeType(&ft)) {
		fprintf(stderr, "Could not init freetype library\n");
	}

	/* Load a font */
	std::string font = "C:/Windows/Fonts/calibril.TTF";
	if (FT_New_Face(ft, font.c_str(), 0, &face)) {
		fprintf(stderr, "Could not open font %s\n", font);
	}


	FT_Set_Pixel_Sizes(face, 0, m_fontSize);
	FT_GlyphSlot g = face->glyph;

	unsigned int roww = 0;
	unsigned int rowh = 0;
	m_width = 0;
	m_height = 0;

	/* Find minimum size for a texture holding all visible ASCII characters */
	for (int i = 32; i < 128; i++) {
		if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			fprintf(stderr, "Loading character %c failed!\n", i);
			continue;
		}
		if (roww + g->bitmap.width + 1 >= MAXWIDTH) {
			m_width = std::max(m_width, roww);
			m_height += rowh;
			roww = 0;
			rowh = 0;
		}
		roww += g->bitmap.width + 1;
		rowh = std::max(rowh, g->bitmap.rows);
	}

	m_width = std::max(m_width, roww);
	m_height += rowh;


	glGenTextures(1, &m_texId);
	glBindTexture(GL_TEXTURE_2D, m_texId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_width, m_height, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

	/* We require 1 byte alignment when uploading texture data */
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/* Paste all glyph bitmaps into the texture, remembering the offset */
	int ox = 0;
	int oy = 0;

	rowh = 0;

	for (int i = 32; i < 128; i++) {
		if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			fprintf(stderr, "Loading character %c failed!\n", i);
			continue;
		}

		if (ox + g->bitmap.width + 1 >= MAXWIDTH) {
			oy += rowh;
			rowh = 0;
			ox = 0;
		}

		glTexSubImage2D(GL_TEXTURE_2D, 0, ox, oy, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);
		c[i].ax = g->advance.x >> 6;
		c[i].ay = g->advance.y >> 6;

		c[i].bw = g->bitmap.width;
		c[i].bh = g->bitmap.rows;

		c[i].bl = g->bitmap_left;
		c[i].bt = g->bitmap_top;

		c[i].tx = ox / (float)m_width;
		c[i].ty = oy / (float)m_height;

		rowh = std::max(rowh, g->bitmap.rows);
		ox += g->bitmap.width + 1;
	}

	fprintf(stderr, "Generated a %d x %d (%d kb) texture atlas\n", m_width, m_height, m_width * m_height / 1024);
}

