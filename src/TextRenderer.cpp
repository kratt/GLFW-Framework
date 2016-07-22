#include "TextRenderer.h"
#include "Shader.h"
#include "VertexBufferObjectAttribs.h"
#include "RenderContext.h"
#include "TextureAtlas.h"

#include <iostream>

#include <glm/vec2.hpp>

TextRenderer* TextRenderer::s_texRendererInstance = nullptr;


TextRenderer::TextRenderer()
  : m_maxNumPriority(20),
	m_fontSize3D(256),
	m_shader(nullptr),
	m_fontPath("C:/Windows/Fonts/")
{
	init();
}

TextRenderer::~TextRenderer()
{
	delete m_shader;

	for (auto s : m_textStrings) {
		delete s;
	}

	for (auto a : m_atlases) {
		delete a;
	}

	m_textStrings.clear();
	m_atlases.clear();
}

void TextRenderer::init()
{
	m_shader = new Shader("../Shader/Text/TextAtlas.vert.glsl", "../Shader/Text/TextAtlas.frag.glsl");
	m_shader->bindAttribLocations();
}

void TextRenderer::render(const std::string &text, const glm::vec2 &pos, unsigned int fontSize, const glm::vec4 &color, const std::string &font)
{
	auto atlas = getTextureAtlas(font, fontSize);
	auto textStr = getTextString(atlas, text);

	auto param = RenderContext::globalObjectParam();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);

	m_shader->bind();

	m_shader->seti("windowWidth", param->windowWidth);
	m_shader->seti("windowHeight", param->windowHeight);
	m_shader->set4f("textColor", color);
	m_shader->set2f("trans", pos);
	m_shader->seti("render3D", false);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, atlas->texId());
	m_shader->seti("texAtlas", 0);

		textStr->vbo->render();

	m_shader->release();
}

void TextRenderer::render(const std::string & text, const glm::vec3 &pos, const glm::vec2 &dims, const glm::vec4 &color, const std::string & font)
{
	auto atlas = getTextureAtlas(font, m_fontSize3D);
	auto textStr = getTextString(atlas, text);

	auto param = RenderContext::globalObjectParam();
	auto trans = RenderContext::transform();

	glm::mat4 view = trans->view;
	glm::mat4 projection = trans->projection;


	float scale_x = dims.x / textStr->width;
	float scale_y = dims.y / textStr->height;

	float scale = 1.0f;
	if (dims.x == 0.0f)      scale = scale_y;
	else if (dims.y == 0.0f) scale = scale_x;
	else                     scale = std::min(scale_x, scale_y);

	glEnable(GL_MULTISAMPLE);

	glm::mat4 matScale = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, 1.0f));
	glm::mat4 matTransPos = glm::translate(glm::mat4(1.0f), pos);
	glm::mat4 model = matTransPos * matScale;


	m_shader->bind();

	m_shader->setMatrix("matModel", model, GL_TRUE);
	m_shader->setMatrix("matView", view, GL_TRUE);
	m_shader->setMatrix("matProjection", projection, GL_TRUE);
	m_shader->set4f("textColor", color);
	m_shader->seti("render3D", true);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, atlas->texId());
	m_shader->seti("texAtlas", 0);

	textStr->vbo->render();

	m_shader->release();
}

TextureAtlas * TextRenderer::getTextureAtlas(const std::string & font, unsigned int fontSize)
{
	// first check if we already have a texture atlas
	auto iter = std::find_if(m_atlases.begin(), m_atlases.end(),
		[&](const TextureAtlas* s)
	{
		return (s->fontSize() == fontSize &&
			font.compare(s->font()) == 0);
	});

	TextureAtlas * ptr = nullptr;
	if (iter == m_atlases.end())
	{
		// init new atlas
		ptr = new TextureAtlas(font, fontSize);
		m_atlases.push_back(ptr);
		std::cout << "TextRenderer. Added new texture atlas. Font: " << font << " (" << fontSize
			<< "), " << ptr->width() << " x " << ptr->height() << "(" << ptr->width() *  ptr->height() / 1024 << " kb)" << std::endl;
	}
	else
	{
		ptr = *iter;
	}

	return ptr;
}

TextString * TextRenderer::getTextString(TextureAtlas *atlas, const std::string &text)
{
	// first check if we already have a StringText object for the text
	auto iter = std::find_if(m_textStrings.begin(), m_textStrings.end(),
		[&](const TextString *s)
	{
		return (s->fontSize == atlas->fontSize() &&
			s->font.compare(atlas->font()) == 0 &&
			text.compare(s->text) == 0);
	});

	TextString *ptr = nullptr;
	if (iter == m_textStrings.end())
	{
		//init new text
		ptr = createTextString(atlas, text);

		// check if the priority queue ist already full, if yes remove last first and delete it
		if (m_textStrings.size() == m_maxNumPriority) {

			delete m_textStrings.back();
			m_textStrings.erase(m_textStrings.end() - 1);
		}

		m_textStrings.push_back(ptr);
	}
	else
	{
		ptr = *iter;
		if (iter != m_textStrings.begin()) {
			std::iter_swap(iter, iter-1);
		}
	}

	return ptr;
}

TextString *TextRenderer::createTextString(TextureAtlas *atlas, const std::string &text)
{
	TextString *textStr = new TextString();
	textStr->font = atlas->font();
	textStr->fontSize = atlas->fontSize();
	textStr->text = text;
	textStr->vbo = nullptr;

	// prepare vbo data
	unsigned int numVertices = 6 * text.size();
	VertexBufferObjectAttribs::DATA *data = new VertexBufferObjectAttribs::DATA[numVertices];

	float sx = 1.0f;
	float sy = 1.0f;

	float w_atlas = atlas->width();
	float h_atlas = atlas->height();

	float x = 0.0f;
	float y = 0.0f;

	float totalWidth = 0.0f;
	float maxHeight = 0.0f;

	unsigned int idx = 0;
	/* Loop through all characters */
	for (auto iter = text.begin(); iter != text.end(); ++iter)
	{
		auto metrics = atlas->metrics(*iter);

		/* Calculate the vertex and texture coordinates */
		float x2 = x + metrics.bl * sx;
		float y2 = -y - metrics.bt * sy;

		float w = metrics.bw * sx;
		float h = metrics.bh * sy;

		float tx = metrics.tx;
		float ty = metrics.ty;

		totalWidth += w;
		maxHeight = std::max(maxHeight, w);

		x += metrics.ax * sx;
		y += metrics.ay * sy;

		if (!w || !h)
			continue;

		data[idx].vx = x2;
		data[idx].vy = -y2;
		data[idx].tx = tx;
		data[idx].ty = ty;

		data[idx + 1].vx = x2 + w;
		data[idx + 1].vy = -y2;
		data[idx + 1].tx = tx + metrics.bw / w_atlas;
		data[idx + 1].ty = ty;


		data[idx + 2].vx = x2;
		data[idx + 2].vy = -y2 - h;
		data[idx + 2].tx = tx;
		data[idx + 2].ty = ty + metrics.bh / h_atlas;


		data[idx + 3].vx = x2 + w;
		data[idx + 3].vy = -y2;
		data[idx + 3].tx = tx + metrics.bw / w_atlas;
		data[idx + 3].ty = ty;


		data[idx + 4].vx = x2;
		data[idx + 4].vy = -y2 - h;
		data[idx + 4].tx = tx;
		data[idx + 4].ty = ty + metrics.bh / h_atlas;

		data[idx + 5].vx = x2 + w;
		data[idx + 5].vy = -y2 - h;
		data[idx + 5].tx = tx + metrics.bw / w_atlas;
		data[idx + 5].ty = ty + metrics.bh / h_atlas;

		idx += 6;
	}

	textStr->width = totalWidth;
	textStr->height = maxHeight;

	delete textStr->vbo;
	textStr->vbo = new VertexBufferObjectAttribs();
	textStr->vbo->setData(data, GL_STATIC_DRAW, numVertices, GL_TRIANGLES);

	textStr->vbo->addAttrib(VERTEX_POSITION);
	textStr->vbo->addAttrib(VERTEX_NORMAL);
	textStr->vbo->addAttrib(VERTEX_COLOR);
	textStr->vbo->addAttrib(VERTEX_TEXTURE);
	textStr->vbo->bindAttribs();

	delete[] data;

	return textStr;
}

TextRenderer* TextRenderer::instance()
{
	if (s_texRendererInstance == nullptr)
		s_texRendererInstance = new TextRenderer();

	return s_texRendererInstance;
}