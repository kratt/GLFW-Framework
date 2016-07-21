//#include "TextString.h"
//#include "text_utils.h"
//#include "image_utils.h"
//#include "opengl_utils.h"
//
//#include <iostream>
//#include <algorithm>
//#include <glm/gtx/norm.hpp>
//
//TextString::TextString(std::string text, int fontSize, const std::string &font)
//:   m_text(text),
//    m_font(font),
//    m_fontSize(fontSize),
//	m_fontPath("C:/Windows/Fonts/"),
//	m_offsetBaseline(0.0f)
//{
//	initTexture();
//	//initTextureSdf();
//}
//
//TextString::~TextString()
//{
//}
//
//GLuint TextString::texId() const
//{
//	return m_texId;
//}
//
//glm::vec2 TextString::dims() const
//{
//	return m_dims;
//}
//
//float TextString::offsetBaseline() const
//{
//	return m_offsetBaseline;
//}
//
//void TextString::initTexture()
//{
//	FT_Library ft;
//	FT_Face face;
//
//	if (FT_Init_FreeType(&ft)) {
//		fprintf(stderr, "Could not init freetype library\n");
//	}
//
//	/* Load a font */
//	std::string font = m_fontPath + m_font + ".TTF";
//	if (FT_New_Face(ft, font.c_str(), 0, &face)) {
//		fprintf(stderr, "Could not open font %s\n", m_font);
//	}
//
//	FT_Set_Pixel_Sizes(face, 0, m_fontSize);
//
//	const char *p;
//	FT_GlyphSlot g = face->glyph;
//
//
//	int totalWidth = 0;
//	int maxHeight  = 0;
//	int maxOffset  = 0;
//
//	int maxTop    = 0;
//	int maxBottom = 0;
//
//	int pen_x = 0;
//	for (p = m_text.c_str(); *p; p++)
//	{
//		if (FT_Load_Char(face, *p, FT_LOAD_RENDER | FT_LOAD_NO_BITMAP))
//		//if (FT_Load_Char(face, *p, FT_LOAD_RENDER | FT_LOAD_MONOCHROME | FT_LOAD_TARGET_MONO))
//			continue;
//
//		int w = g->bitmap.width;
//		int h = g->bitmap.rows;
//
//		int pos_x = pen_x + g->bitmap_left;
//		int belowBaseLine = h - g->bitmap_top;
//
//		totalWidth = pen_x + w;
//		pen_x += g->advance.x >> 6;
//
//		maxTop = std::max(maxTop, g->bitmap_top);
//		maxBottom = std::max(maxBottom, h - g->bitmap_top);
//		maxOffset = std::max(maxOffset, belowBaseLine);
//
//		//if (g->bitmap.pixel_mode == ft_pixel_mode_mono)
//		//	std::cout << "ft_pixel_mode_mono" << std::endl;
//		//if (g->bitmap.pixel_mode == ft_pixel_mode_grays)
//		//	std::cout << "ft_pixel_mode_grays (8Bit)" << std::endl;
//	}
//
//	maxHeight = maxTop + maxBottom;
//
//	m_dims = glm::vec2(totalWidth, maxHeight);
//	m_offsetBaseline = maxOffset;
//
//	/* Create an empty texture that will be used to hold the entire text */
//
//	glGenTextures(1, &m_texId);
//	glBindTexture(GL_TEXTURE_2D, m_texId);
//	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, totalWidth, maxHeight, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
//
//	pen_x = 0;
//	/* Loop through all characters */
//	for (auto it = m_text.begin(); it < m_text.end(); it++)
//	{
//		if (FT_Load_Char(face, *it, FT_LOAD_RENDER | FT_LOAD_NO_BITMAP))
//		//if (FT_Load_Char(face, *it, FT_LOAD_RENDER | FT_LOAD_MONOCHROME | FT_LOAD_TARGET_MONO))
//			continue;
//
//		float w = g->bitmap.width;
//		float h = g->bitmap.rows;
//		float top_left = g->bitmap_top;	
//
//		float pos_x = pen_x + g->bitmap_left;
//
//		int x = pen_x;
//		int y = maxHeight - (g->bitmap_top + m_offsetBaseline);
//		int tex_w = g->bitmap.width;
//		int tex_h = g->bitmap.rows;
//
//		glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, tex_w, tex_h, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);
//		pen_x += g->advance.x >> 6;
//	}
//	glBindTexture(GL_TEXTURE_2D, 0);
//
//	utils::save_texture("./../A.png", m_texId);
//}
//
//void TextString::initTextureSdf()
//{
//	FT_Library ft;
//	FT_Face face;
//
//	if (FT_Init_FreeType(&ft)) {
//		fprintf(stderr, "Could not init freetype library\n");
//	}
//
//	/* Load a font */
//	std::string font = m_fontPath + m_font + ".TTF";
//	if (FT_New_Face(ft, font.c_str(), 0, &face)) {
//		fprintf(stderr, "Could not open font %s\n", m_font);
//	}
//
//	FT_Set_Pixel_Sizes(face, 0, m_fontSize);
//
//	const char *p;
//	FT_GlyphSlot g = face->glyph;
//
//
//	/* Create a texture that will be used to hold one "glyph" */
//	glActiveTexture(GL_TEXTURE0);
//	glGenTextures(1, &m_texId);
//	glBindTexture(GL_TEXTURE_2D, m_texId);
//	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//	int pen_x = 0;
//	int pen_y = 0;
//
//	int totalWidth = 0.0f;
//	int maxHeight = 0.0f;
//
//	/* Loop through all characters */
//	for (p = m_text.c_str(); *p; p++)
//	{
//		if (FT_Load_Char(face, *p, FT_LOAD_RENDER | FT_LOAD_MONOCHROME | FT_LOAD_TARGET_MONO))
//			continue;
//
//		int border = 0;
//
//		std::vector<float> sdfData;
//		utils::distance_field_linear(&g->bitmap, border, sdfData);
//
//		std::vector<unsigned char> bitmapData;
//		utils::unpack_mono_bitmap(&g->bitmap, bitmapData);
//
//
//		int width  = g->bitmap.width + 2 * border;
//		int height = g->bitmap.rows  + 2 * border;
//
//		glTexStorage2D(GL_TEXTURE_2D, 4, GL_R32F, width, height);
//		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RED, GL_FLOAT, sdfData.data());
//		glGenerateMipmap(GL_TEXTURE_2D);
//
//		//glTexImage2D(GL_TEXTURE_2D, 2, GL_RED, g->bitmap.width, g->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, sdfData);
//
//		float w = g->bitmap.width;
//		float h = g->bitmap.rows;
//
//		float pos_x = pen_x + g->bitmap_left;
//		float pos_y = pen_y + g->bitmap_top - h;
//
//		pen_x += g->advance.x >> 6;
//		pen_y += g->advance.y >> 6;
//
//		maxHeight = h;
//		totalWidth = w;
//		break;
//	}
//
//	m_dims = glm::vec2(totalWidth, maxHeight);
//	glBindTexture(GL_TEXTURE_2D, 0);
//}
//
//int TextString::fontSize() const
//{
//	return m_fontSize;
//}
//
//std::string TextString::font() const
//{
//	return m_font;
//}
//
//std::string TextString::text() const
//{
//	return m_text;
//}
