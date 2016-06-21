#include "TextRenderer.h"
#include "Shader.h"
#include "Mesh.h"
#include "VertexBufferObjectAttribs.h"
#include "RenderContext.h"
#include "Texture.h"

#include <iostream>

TextRenderer* TextRenderer::s_texRendererInstance = nullptr;

TextRenderer::TextRenderer()
{
	init();

	m_texTest = new Texture("../Data/Textures/floor_test.png");
}

void TextRenderer::init()
{
	if (FT_Init_FreeType(&ft)) {
		fprintf(stderr, "Could not init freetype library\n");
	}

	/* Load a font */
	if (FT_New_Face(ft, "C:/Windows/Fonts/Calibri.ttf", 0, &face)) {
		fprintf(stderr, "Could not open font %s\n", "Calibri");
	}


	m_shaderText = new Shader("../Shader/Text.vert.glsl", "../Shader/Text.frag.glsl");
	m_shaderText->bindAttribLocations();

	// quad to render one glyph. Sscale it according to needs
	m_vboQuad = Mesh::quad(0, 0, 1, 1);
}


void TextRenderer::renderText(int x, int y, const char *text)
{
	FT_Set_Pixel_Sizes(face, 0, 48);

	const char *p;
	FT_GlyphSlot g = face->glyph;

	/* Create a texture that will be used to hold one "glyph" */
	GLuint texId;

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);

	/* We require 1 byte alignment when uploading texture data */
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	/* Clamping to edges is important to prevent artifacts when scaling */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	/* Linear filtering usually looks best for text */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	auto param = RenderContext::globalObjectParam();
	float sx = 2.0f / float(param->windowWidth);
	float sy = 2.0f / float(param->windowHeight);

	
	/* Loop through all characters */
	for (p = text; *p; p++) {
		/* Try to load and render the character */
		if (FT_Load_Char(face, 'k', FT_LOAD_RENDER))
			continue;

		/* Upload the "bitmap", which contains an 8-bit grayscale image, as an alpha texture */
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, g->bitmap.width, g->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

	
		/* Calculate the vertex and texture coordinates */
		float x2 = x + g->bitmap_left * sx;
		float y2 = -y - g->bitmap_top * sy;
		float w = g->bitmap.width;// *sx;
		float h = g->bitmap.rows; // *sy;

		std::cout << x2 << " " << y2 << " " << w << " " << h << std::endl;

	/*	point box[4] = {
			{ x2, -y2, 0, 0 },
			{ x2 + w, -y2, 1, 0 },
			{ x2, -y2 - h, 0, 1 },
			{ x2 + w, -y2 - h, 1, 1 },
		};*/


		glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, 100.0f, 0.0f));
		glm::mat4 model = glm::scale(trans, glm::vec3(float(h), float(w), 1.0f));
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		glm::mat4 projection = glm::ortho(0.0f, float(param->windowWidth), 0.0f, float(param->windowHeight), -1.0f, 1.0f);

		glDisable(GL_DEPTH_TEST);

		m_shaderText->bind();

		m_shaderText->setMatrix("matModel", model, GL_TRUE);
		m_shaderText->setMatrix("matView", view, GL_TRUE);
		m_shaderText->setMatrix("matProjection", projection, GL_TRUE);

		m_shaderText->set2f("windowSize", param->windowWidth, param->windowHeight);
		m_shaderText->set2f("glyphSize", w, h);
		m_shaderText->set2f("glyphPos", x2, y2);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texId);
		m_shaderText->seti("tex", 0);

		m_vboQuad->render();

		m_shaderText->release();
		glEnable(GL_DEPTH_TEST);


		/* Advance the cursor to the start of the next character */
		x += (g->advance.x >> 6) * sx;
		y += (g->advance.y >> 6) * sy;
	}



}

TextRenderer* TextRenderer::instance()
{
	if (s_texRendererInstance == nullptr)
		s_texRendererInstance = new TextRenderer();

	return s_texRendererInstance;
}