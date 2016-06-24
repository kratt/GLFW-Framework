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

	m_texTest = new Texture("../Data/Textures/floor_blue.png");
}

void TextRenderer::init()
{
	if (FT_Init_FreeType(&ft)) {
		fprintf(stderr, "Could not init freetype library\n");
	}

	/* Load a font */
	if (FT_New_Face(ft, "C:/Windows/Fonts/Calibri.TTF", 0, &face)) {
		fprintf(stderr, "Could not open font %s\n", "Calibri");
	}


	m_shaderText = new Shader("../Shader/Text.vert.glsl", "../Shader/Text.frag.glsl");
	m_shaderText->bindAttribLocations();

	// quad to render one glyph. Sscale it according to needs
	m_vboQuad = Mesh::quad(0, 0, 1, 1);
}


void TextRenderer::renderText(int x, int y, const char *text)
{
	FT_Set_Pixel_Sizes(face, 0, 24);

	const char *p;
	FT_GlyphSlot g = face->glyph;


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_DEPTH_TEST);

	/* Create a texture that will be used to hold one "glyph" */
	GLuint texId;

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	auto param = RenderContext::globalObjectParam();

	int pen_x = x;
	int pen_y = y;

	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 projection = glm::ortho(0.0f, float(param->windowWidth), 0.0f, float(param->windowHeight), -1.0f, 1.0f);

	m_shaderText->bind();

	/* Loop through all characters */
	for (p = text; *p; p++) 
	{
		if (FT_Load_Char(face, *p, FT_LOAD_RENDER))
			continue;

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, g->bitmap.width, g->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

		float w = g->bitmap.width;
		float h = g->bitmap.rows;
		
		float pos_x = pen_x + g->bitmap_left;
		float pos_y = pen_y + g->bitmap_top - h;

		glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(pos_x, pos_y, 0.0f));
		glm::mat4 model = glm::scale(trans, glm::vec3(float(w), float(h), 1.0f));	

		m_shaderText->setMatrix("matModel", model, GL_TRUE);
		m_shaderText->setMatrix("matView", view, GL_TRUE);
		m_shaderText->setMatrix("matProjection", projection, GL_TRUE);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texId);
		m_shaderText->seti("tex", 0);

		m_vboQuad->render();

		pen_x += g->advance.x >> 6;
		pen_y += g->advance.y >> 6;
	}

	m_shaderText->release();
	glEnable(GL_DEPTH_TEST);
}

void TextRenderer::render(TextString * text)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_DEPTH_TEST);


	auto param = RenderContext::globalObjectParam();

	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 projection = glm::ortho(0.0f, float(param->windowWidth), 0.0f, float(param->windowHeight), -1.0f, 1.0f);

	glm::vec2 dims = text->dims();
	glm::vec2 pos = text->pos();

//	std::cout << text->texId() << " " << dims.x << " " << dims.y << ", " << pos.x << " " << pos.y << std::endl;

	m_shaderText->bind();

	glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, 0.0f));
	glm::mat4 model = glm::scale(trans, glm::vec3(float(dims.x), float(dims.y), 1.0f));

	m_shaderText->setMatrix("matModel", model, GL_TRUE);
	m_shaderText->setMatrix("matView", view, GL_TRUE);
	m_shaderText->setMatrix("matProjection", projection, GL_TRUE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, text->texId());
	m_shaderText->seti("tex", 0);

	m_vboQuad->render();

	m_shaderText->release();
	glEnable(GL_DEPTH_TEST);
}

TextRenderer* TextRenderer::instance()
{
	if (s_texRendererInstance == nullptr)
		s_texRendererInstance = new TextRenderer();

	return s_texRendererInstance;
}