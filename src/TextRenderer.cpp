#include "TextRenderer.h"
#include "Shader.h"
#include "Mesh.h"
#include "VertexBufferObjectAttribs.h"
#include "RenderContext.h"
#include "Texture.h"

#include <iostream>

#include <glm/gtx/vector_angle.hpp>

TextRenderer* TextRenderer::s_texRendererInstance = nullptr;

TextRenderer::TextRenderer()
{
	init();
}

TextRenderer::~TextRenderer()
{
	delete s_texRendererInstance;
}

void TextRenderer::init()
{
	m_shaderText = new Shader("../Shader/Text.vert.glsl", "../Shader/Text.frag.glsl");
	m_shaderText->bindAttribLocations();

	m_shaderTextSdf = new Shader("../Shader/TextSdf.vert.glsl", "../Shader/TextSdf.frag.glsl");
	m_shaderTextSdf->bindAttribLocations();

	// quad to render one glyph. Sscale it according to needs
	m_vboQuad = Mesh::quad(0, 0, 1, 1);
}

void TextRenderer::render(const std::string &text, glm::vec2 pos, int fontSize, const std::string &font)
{
	auto textStr = getTextString(text, font, fontSize);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_DEPTH_TEST);

	auto param = RenderContext::globalObjectParam();

	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 projection = glm::ortho(0.0f, float(param->windowWidth), 0.0f, float(param->windowHeight), -1.0f, 1.0f);

	glm::vec2 dims = textStr->dims();

	float border = 0.0f;

	m_shaderText->bind();

	glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(pos, 0.0f));
	glm::mat4 model = glm::scale(trans, glm::vec3(float(dims.x) + 2.0f*border, float(dims.y) + 2.0f*border, 1.0f));

	m_shaderText->setMatrix("matModel", model, GL_TRUE);
	m_shaderText->setMatrix("matView", view, GL_TRUE);
	m_shaderText->setMatrix("matProjection", projection, GL_TRUE);
	m_shaderText->set2f("textDims", dims);
	m_shaderText->seti("faceToCamera", false);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textStr->texId());
	m_shaderText->seti("tex", 0);

	m_vboQuad->render();

	m_shaderText->release();
	glEnable(GL_DEPTH_TEST);
}


void TextRenderer::render3d(const std::string &text, glm::vec3 pos, int fontSize, const std::string &font)
{
	auto textStr = getTextString(text, font, fontSize);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_CULL_FACE);

	auto param = RenderContext::globalObjectParam();
	auto trans = RenderContext::transform();

	glm::mat4 view = trans->view;
	glm::mat4 projection = trans->projection;

	float scale = 0.01f;

	glm::vec2 dims = textStr->dims();
	glm::vec3 center = 0.5f * glm::vec3(float(dims.x), float(dims.y), 0.0f);

	glm::mat4 matScale = glm::scale(glm::mat4(1.0f), scale*glm::vec3(float(dims.x), float(dims.y), 1.0f));
	glm::mat4 matTransPos = glm::translate(glm::mat4(1.0f), pos);

	m_shaderText->bind();
	
	glm::mat4 model = matTransPos * matScale;

	m_shaderText->setMatrix("matModel", model, GL_TRUE);
	m_shaderText->setMatrix("matView", view, GL_TRUE);
	m_shaderText->setMatrix("matProjection", projection, GL_TRUE);
	m_shaderText->set2f("textDims", dims);
	m_shaderText->seti("faceToCamera", true);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textStr->texId());
	m_shaderText->seti("tex", 0);

	m_vboQuad->render();

	m_shaderText->release();
}

TextString * TextRenderer::getTextString(const std::string &text, const std::string &font, int fontSize)
{		
	// first check if we already have a StringText object for the text
	auto iter =	std::find_if(m_textStrings.begin(), m_textStrings.end(),
		[&](const TextString* s) 
	{ 
		return (s->fontSize() == fontSize &&
			    font.compare(s->font()) == 0 &&
			    text.compare(s->text()) == 0);
	});

	TextString * resPrt = nullptr;
	if (iter == m_textStrings.end())
	{
		//init new text
		resPrt = new TextString(text, fontSize, font);
		m_textStrings.push_back(resPrt);

		std::cout << "TextRenderer. Added new text string." << std::endl;
	}
	else
	{
		resPrt = *iter;
	}

	return resPrt;
}

void TextRenderer::renderSdf(const std::string &text, glm::vec2 pos, int fontSize, const std::string &font)
{
	auto textStr = getTextString(text, font, fontSize);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_DEPTH_TEST);

	auto param = RenderContext::globalObjectParam();

	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 projection = glm::ortho(0.0f, float(param->windowWidth), 0.0f, float(param->windowHeight), -1.0f, 1.0f);

	glm::vec2 dims = textStr->dims();
	float scale = 0.1;

	m_shaderTextSdf->bind();

	glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(pos, 0.0f));
	glm::mat4 model = glm::scale(trans, scale * glm::vec3(float(dims.x), float(dims.y), 1.0f));

	m_shaderTextSdf->setMatrix("matModel", model, GL_TRUE);
	m_shaderTextSdf->setMatrix("matView", view, GL_TRUE);
	m_shaderTextSdf->setMatrix("matProjection", projection, GL_TRUE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textStr->texId());
	m_shaderTextSdf->seti("tex", 0);

	m_vboQuad->render();

	m_shaderTextSdf->release();
	glEnable(GL_DEPTH_TEST);
}

TextRenderer* TextRenderer::instance()
{
	if (s_texRendererInstance == nullptr)
		s_texRendererInstance = new TextRenderer();

	return s_texRendererInstance;
}