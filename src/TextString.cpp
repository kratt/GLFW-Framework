#include "TextString.h"

#include <iostream>
#include <algorithm>
#include <glm/gtx/norm.hpp>

TextString::TextString(std::string text, int fontSize, const std::string &font)
:   m_text(text),
    m_font(font),
    m_fontSize(fontSize),
	m_fontPath("C:/Windows/Fonts/"),
	m_offsetBaseline(0.0f)
{
	initTexture();
	//initTextureSdf();
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

float TextString::offsetBaseline() const
{
	return m_offsetBaseline;
}

void TextString::initTexture()
{
	FT_Library ft;
	FT_Face face;

	if (FT_Init_FreeType(&ft)) {
		fprintf(stderr, "Could not init freetype library\n");
	}

	/* Load a font */
	std::string font = m_fontPath + m_font + ".TTF";
	if (FT_New_Face(ft, font.c_str(), 0, &face)) {
		fprintf(stderr, "Could not open font %s\n", m_font);
	}

	FT_Set_Pixel_Sizes(face, 0, m_fontSize);

	const char *p;
	FT_GlyphSlot g = face->glyph;


	int totalWidth = 0;
	int maxHeight  = 0;
	int maxOffset  = 0;

	int maxTop    = 0;
	int maxBottom = 0;

	int pen_x = 0;
	for (p = m_text.c_str(); *p; p++)
	{
		if (FT_Load_Char(face, *p, FT_LOAD_RENDER))
			continue;

		int w = g->bitmap.width;
		int h = g->bitmap.rows;

		int pos_x = pen_x + g->bitmap_left;
		int belowBaseLine = h - g->bitmap_top;

		totalWidth = pen_x + w;
		pen_x += g->advance.x >> 6;

		maxTop = std::max(maxTop, g->bitmap_top);
		maxBottom = std::max(maxBottom, h - g->bitmap_top);
		maxOffset = std::max(maxOffset, belowBaseLine);
	}

	maxHeight = maxTop + maxBottom;


	m_dims = glm::vec2(totalWidth, maxHeight);
	m_offsetBaseline = maxOffset;

	std::cout << "dims: " << m_dims.x << " " << m_dims.y << std::endl;

	/* Create an empty texture that will be used to hold the entire text */
	glGenTextures(1, &m_texId);
	glBindTexture(GL_TEXTURE_2D, m_texId);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, totalWidth, maxHeight, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

	pen_x = 0;
	/* Loop through all characters */
	for (p = m_text.c_str(); *p; p++)
	{
		if (FT_Load_Char(face, *p, FT_LOAD_RENDER))
			continue;

		float w = g->bitmap.width;
		float h = g->bitmap.rows;
		float top_left = g->bitmap_top;

		float pos_x = pen_x + g->bitmap_left;
		glTexSubImage2D(GL_TEXTURE_2D, 0, pen_x, maxHeight - (g->bitmap_top + m_offsetBaseline), g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);
		pen_x += g->advance.x >> 6;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}


void TextString::initTextureSdf()
{
	FT_Library ft;
	FT_Face face;

	if (FT_Init_FreeType(&ft)) {
		fprintf(stderr, "Could not init freetype library\n");
	}

	/* Load a font */
	std::string font = m_fontPath + m_font + ".TTF";
	if (FT_New_Face(ft, font.c_str(), 0, &face)) {
		fprintf(stderr, "Could not open font %s\n", m_font);
	}

	FT_Set_Pixel_Sizes(face, 0, m_fontSize);

	const char *p;
	FT_GlyphSlot g = face->glyph;


	/* Create a texture that will be used to hold one "glyph" */
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &m_texId);
	glBindTexture(GL_TEXTURE_2D, m_texId);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int pen_x = 0;
	int pen_y = 0;

	int totalWidth = 0.0f;
	int maxHeight = 0.0f;

	/* Loop through all characters */
	for (p = m_text.c_str(); *p; p++)
	{
		if (FT_Load_Char(face, *p, FT_LOAD_RENDER | FT_LOAD_MONOCHROME | FT_LOAD_TARGET_MONO))
			continue;

		unsigned char* sdfData;
		distanceField(&g->bitmap, &sdfData);

		glTexStorage2D(GL_TEXTURE_2D, 1, GL_R8, g->bitmap.width, g->bitmap.rows);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, sdfData);
		glGenerateMipmap(GL_TEXTURE_2D);
		//glTexImage2D(GL_TEXTURE_2D, 2, GL_RED, g->bitmap.width, g->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, sdfData);

		float w = g->bitmap.width;
		float h = g->bitmap.rows;

		float pos_x = pen_x + g->bitmap_left;
		float pos_y = pen_y + g->bitmap_top - h;

		pen_x += g->advance.x >> 6;
		pen_y += g->advance.y >> 6;

		maxHeight = h;
		totalWidth = w;
		break;
	}

	std::cout << "width: " << totalWidth << " , height: " << maxHeight << std::endl;

	
	m_dims = glm::vec2(totalWidth, maxHeight);
	
	glBindTexture(GL_TEXTURE_2D, 0);
}


void TextString::distanceField(FT_Bitmap* bitmap, unsigned char** outData)
{
	// first unpack monochromatic bitmap
	unsigned char* bitMapData;
	unpackMonoBitmap(bitmap, &bitMapData);

	int width = bitmap->width;
	int height = bitmap->rows;

	float* sdf = new float[width * height];

	float minDist = std::numeric_limits<float>::max();
	float maxDist = std::numeric_limits<float>::lowest();

	float spread = 4.0f;

	//compute signed distance field
	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			glm::vec2 pos = glm::vec2(x, y);
			auto val = bitMapData[y*width + x];

			// inside texels (white) have a negative sign
			float sign = val > 0 ? -1.0f : 1.0f;

			// found nearest opposite texel
			float dist = std::numeric_limits<float>::max();

			bool found = false;
			for (int s = 0; s < width; ++s)
			{
				for (int t = 0; t < height; ++t)
				{
					glm::vec2 curPos = glm::vec2(s, t);
					auto curVal = bitMapData[t*width + s];

					if (curVal == val)
						continue;

					float curDist = glm::length(curPos - pos);

					//if (curDist < spread) 
					{

						dist = std::min(dist, curDist);
						found = true;
					}
				}
			}

			float newVal = sign * dist;
			sdf[y*width + x] = newVal;

			minDist = std::min(minDist, newVal);
			maxDist = std::max(maxDist, newVal);
	/*		if (found) {
				float newVal = sign * dist;
				sdf[y*width + x] = newVal;

				minDist = std::min(minDist, newVal);
				maxDist = std::max(maxDist, newVal);
			}
			else {
				float newVal = sign < 0 ? 0.0f : 255.0f;
				sdf[y*width + x] = newVal;
			}	*/
		}
	}

	std::cout << "min: " << minDist << " , max:" << maxDist << std::endl;

	// normalize data
	unsigned char* finalData = new unsigned char[width * height];
	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			int idx = y*width + x;
			float val = sdf[idx];

			finalData[idx] = (sdf[idx] - minDist) / (maxDist - minDist) * 255;
/*
			if (val != 0.0 && val != 255.0) {
				finalData[idx] = (sdf[idx] - minDist) / (maxDist - minDist) * 255;
			}	*/	
		}
	}


	//// normalize data
	//float rangeStart = 0.0f;
	//float rangeMid = 192.0f;
	//float rangeEnd = 255.0f;

	//unsigned char* finalData = new unsigned char[width * height];
	//for (int x = 0; x < width; ++x)
	//{
	//	for (int y = 0; y < height; ++y)
	//	{
	//		int idx = y*width + x;
	//		float val = sdf[idx];
	//		
	//		float newVal = 0.0f;

	//		// val between -1 and 1
	//		if (val < 0.0f) {
	//			val /= minDist;
	//			newVal = std::abs(val) * (rangeEnd - rangeMid) + rangeMid;
	//		}
	//		else {
	//			val /= maxDist;
	//			newVal = val * rangeMid;
	//		}
	//				
	//		finalData[idx] = newVal; // (sdf[idx] - minDist) / (maxDist - minDist) * 255;
	//	}
	//}

	delete[] sdf;
	delete[] bitMapData;
	*outData = finalData;
}


void TextString::unpackMonoBitmap(FT_Bitmap* bitmap, unsigned char** outData)
{	
	int width = bitmap->width;
	int height = bitmap->rows;

	GLubyte *data = new unsigned char[width * height];

	for (int y = 0; y < bitmap->rows; ++y)
	{
		for (int byteIndex = 0; byteIndex < bitmap->pitch; ++byteIndex)
		{
			auto byte_value = bitmap->buffer[y * bitmap->pitch + byteIndex];
			int num_bits_done = byteIndex * 8;
			int row_start = y * bitmap->width + byteIndex * 8;

			for (int bitIndex = 0; bitIndex < std::min(8, bitmap->width - num_bits_done); ++bitIndex)
			{
				auto bit = byte_value & (1 << (7 - bitIndex));

				if(bit)
					data[row_start + bitIndex] = 255;
				else
					data[row_start + bitIndex] = 0;
			}
		}
	}

	*outData = data;
}

void TextString::emptyTexture(FT_Bitmap* bitmap, unsigned char** outData)
{
	int width = bitmap->width;
	int height = bitmap->rows;

	GLubyte *data = new unsigned char[width * height];

	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			data[y*width + x] = 255;
		}
	}

	*outData = data;
}

int TextString::fontSize() const
{
	return m_fontSize;
}

std::string TextString::font() const
{
	return m_font;
}

std::string TextString::text() const
{
	return m_text;
}
