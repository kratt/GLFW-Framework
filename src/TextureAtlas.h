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

#define MAXWIDTH 1024

struct TextureAtlas
{
	GLuint tex;		// texture object

	int width;			// width of texture in pixels
	int height;	    // height of texture in pixels

	struct {
		float ax;	// advance.x
		float ay;	// advance.y

		float bw;	// bitmap.width;
		float bh;	// bitmap.height;

		float bl;	// bitmap_left;
		float bt;	// bitmap_top;

		float tx;	// x offset of glyph in texture coordinates
		float ty;	// y offset of glyph in texture coordinates
	} c[128];


	TextureAtlas(int height) 
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


		FT_Set_Pixel_Sizes(face, 0, height);
		FT_GlyphSlot g = face->glyph;


		int totalWidth = 0;
		int maxHeight = 0;
		int maxOffset = 0;

		int maxTop = 0;
		int maxBottom = 0;

		std::string text = "hallo";
		int pen_x = 0;
		for (auto p = text.c_str(); *p; p++)
		{
			if (FT_Load_Char(face, *p, FT_LOAD_RENDER | FT_LOAD_NO_BITMAP))
				//if (FT_Load_Char(face, *p, FT_LOAD_RENDER | FT_LOAD_MONOCHROME | FT_LOAD_TARGET_MONO))
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

		glm::vec2 dims = glm::vec2(totalWidth, maxHeight);

		/* Create an empty texture that will be used to hold the entire text */

		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, totalWidth, maxHeight, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

		pen_x = 0;
		/* Loop through all characters */
		for (auto it = text.begin(); it < text.end(); it++)
		{
			if (FT_Load_Char(face, *it, FT_LOAD_RENDER | FT_LOAD_NO_BITMAP))
				//if (FT_Load_Char(face, *it, FT_LOAD_RENDER | FT_LOAD_MONOCHROME | FT_LOAD_TARGET_MONO))
				continue;

			float w = g->bitmap.width;
			float h = g->bitmap.rows;
			float top_left = g->bitmap_top;

			float pos_x = pen_x + g->bitmap_left;

			glTexSubImage2D(GL_TEXTURE_2D, 0, pen_x, 0, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);
			//glTexSubImage2D(GL_TEXTURE_2D, 0, 0,0, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, data.data());
			//glGenerateMipmap(GL_TEXTURE_2D);
			pen_x += g->advance.x >> 6;
		}


		//int roww = 0;
		//int rowh = 0;
		//width = 0;
		//height = 0;

		////memset(c, 0, sizeof c);

		///* Find minimum size for a texture holding all visible ASCII characters */
		//for (int i = 32; i < 128; i++) {
		//	if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
		//		fprintf(stderr, "Loading character %c failed!\n", i);
		//		continue;
		//	}
		//	roww += g->bitmap.width;
		//	rowh = std::max(rowh, g->bitmap.rows);
		//}

		//width = std::max(width, roww);
		//height += rowh;

		//std::cout << "width: " << width << ", height: " << height << std::endl;

		//glGenTextures(1, &tex);
		//glBindTexture(GL_TEXTURE_2D, tex);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		///* Paste all glyph bitmaps into the texture, remembering the offset */
		//int ox = 0;
		//int oy = 0;

		//rowh = 0;

		//for (int i = 32; i < 128; i++) {
		//	if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
		//		fprintf(stderr, "Loading character %c failed!\n", i);
		//		continue;
		//	}

		//	std::cout << ox << " " << oy << " " << g->bitmap.width << " " << g->bitmap.rows << std::endl;

		//	glTexSubImage2D(GL_TEXTURE_2D, 0, ox, oy, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);
		//	c[i].ax = g->advance.x >> 6;
		//	c[i].ay = g->advance.y >> 6;

		//	c[i].bw = g->bitmap.width;
		//	c[i].bh = g->bitmap.rows;

		//	c[i].bl = g->bitmap_left;
		//	c[i].bt = g->bitmap_top;

		//	c[i].tx = ox / (float)width;
		//	c[i].ty = oy / (float)height;

		//	rowh = std::max(rowh, g->bitmap.rows);
		//	ox += g->bitmap.width;
		//}

		/*fprintf(stderr, "Generated a %d x %d (%d kb) texture atlas\n", w, h, w * h / 1024);*/
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	~TextureAtlas() {
		glDeleteTextures(1, &tex);
	}


};

#endif
