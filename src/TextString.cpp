#include "TextString.h"

#include <iostream>
#include <algorithm>

TextString::TextString(std::string text, glm::vec2 screenPos)
	: m_text(text),
	m_pos(screenPos)
{
	initTexture();
}

TextString::~TextString()
{
}

GLuint TextString::texId() const
{
	return m_texId;
}

glm::vec2 TextString::dims() const
{
	return m_dims;
}

glm::vec2 TextString::pos() const
{
	return m_pos;
}

void TextString::initTexture()
{
	FT_Library ft;
	FT_Face face;


	if (FT_Init_FreeType(&ft)) {
		fprintf(stderr, "Could not init freetype library\n");
	}

	/* Load a font */
	if (FT_New_Face(ft, "C:/Windows/Fonts/Calibri.TTF", 0, &face)) {
		fprintf(stderr, "Could not open font %s\n", "Calibri");
	}

	FT_Set_Pixel_Sizes(face, 0, 24);

	const char *p;
	FT_GlyphSlot g = face->glyph;


	// determine min and max dims
	

	//for (p = m_text.c_str(); *p; p++)
	//{
	//	if (FT_Load_Char(face, *p, FT_LOAD_RENDER))
	//		continue;

	//	//totalWidth += g->bitmap.width;
	//	totalWidth += g->advance.x >> 6;
	//	maxHeight = std::max(maxHeight, g->bitmap.rows);
	//}

	int totalWidth = 0.0f;
	int maxHeight = 0.0f;

	int pen_x = 0;
	int pen_y = 0;

	bool firstRun = true;
	float firstDist = 0.0f;

	for (p = m_text.c_str(); *p; p++)
	{
		if (FT_Load_Char(face, *p, FT_LOAD_RENDER | FT_LOAD_MONOCHROME | FT_LOAD_TARGET_MONO))
			continue;

		float w = g->bitmap.width;
		float h = g->bitmap.rows;

		float pos_x = pen_x + g->bitmap_left;
		float pos_y = pen_y + g->bitmap_top - h;

		// update position based on beginning of first character
		if (firstRun) {
			m_pos.x = m_pos.x + g->bitmap_left;
			m_pos.y = m_pos.y + g->bitmap_top - h;

			firstDist = g->bitmap_left;
			firstRun = false;
		}
	
		totalWidth = pen_x + w;

		pen_x += g->advance.x >> 6;
		pen_y += g->advance.y >> 6;
	
		maxHeight = std::max(maxHeight, int(h));
	}

	
//	totalWidth = pen_x;

	m_dims = glm::vec2(totalWidth, maxHeight);

	std::cout << "Text dims: " << m_dims.x << " " << m_dims.y << std::endl;

	/* Create an empty texture that will be used to hold the entire text */
	glGenTextures(1, &m_texId);
	glBindTexture(GL_TEXTURE_2D, m_texId);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, totalWidth, maxHeight, 0, GL_RED, GL_UNSIGNED_BYTE, 0);


	pen_x = 0;
	pen_y = 0;

	/* Loop through all characters */
	for (p = m_text.c_str(); *p; p++)
	{
		if (FT_Load_Char(face, *p, FT_LOAD_RENDER | FT_LOAD_MONOCHROME | FT_LOAD_TARGET_MONO))
			continue;

		float w = g->bitmap.width;
		float h = g->bitmap.rows;

		float pos_x = pen_x + g->bitmap_left;
		float pos_y = maxHeight-pen_y;// +g->bitmap_top;

		glTexSubImage2D(GL_TEXTURE_2D, 0, pen_x, maxHeight-h, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

		pen_x += g->advance.x >> 6;
		pen_y += g->advance.y >> 6;
	}


	///* Loop through all characters */
	//for (p = text; *p; p++)
	//{
	//	float w = g->bitmap.width;
	//	float h = g->bitmap.rows;

	//	float pos_x = pen_x + g->bitmap_left;
	//	float pos_y = pen_y + g->bitmap_top - h;

	//	glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(pos_x, pos_y, 0.0f));
	//	glm::mat4 model = glm::scale(trans, glm::vec3(float(w), float(h), 1.0f));

	//	pen_x += g->advance.x >> 6;
	//	pen_y += g->advance.y >> 6;
	//}



	glBindTexture(GL_TEXTURE_2D, 0);
}
