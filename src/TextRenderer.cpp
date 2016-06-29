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

void TextRenderer::render(const std::string &text, int x, int y, int fontSize, const std::string &font)
{
	auto textStr = getTextString(text, font, fontSize);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_DEPTH_TEST);

	auto param = RenderContext::globalObjectParam();

	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 projection = glm::ortho(0.0f, float(param->windowWidth), 0.0f, float(param->windowHeight), -1.0f, 1.0f);

	glm::vec2 dims = textStr->dims();

	m_shaderText->bind();

	glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f));
	glm::mat4 model = glm::scale(trans, glm::vec3(float(dims.x), float(dims.y), 1.0f));

	m_shaderText->setMatrix("matModel", model, GL_TRUE);
	m_shaderText->setMatrix("matView", view, GL_TRUE);
	m_shaderText->setMatrix("matProjection", projection, GL_TRUE);

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


	//glm::vec3 look = glm::normalize(camera->getPosition() - objectPosition);
	//glm::vec3 right = glm::cross(up, look);
	//glm::vec3 up2 = glm::cross(look, right);
	//glm::mat4 transform;
	//transform[0] = glm::vec4(right, 0);
	//transform[1] = glm::vec4(up2, 0);
	//transform[2] = glm::vec4(look, 0);
	//transform[3] = glm::vec4(objectPosition, 1);


	// get view dir;
	glm::vec4 camPos = glm::inverse(view)[3];
	glm::vec3 viewDir = glm::normalize(glm::vec3(camPos.x, camPos.y, camPos.z));

	glm::vec3 quadNormal = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 quadRight = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 quadUp = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 rotAxis = glm::normalize(glm::cross(quadNormal, viewDir));
	float angle = glm::angle(quadNormal, viewDir);

	//glm::vec4 a = 


	//glm::mat4 matBillboard = glm::mat4(1.0f);
	//matBillboard[0] = glm::vec4(quadRight, 0);
	//matBillboard[1] = glm::vec4(quadUp, 0);
	//matBillboard[2] = glm::vec4(quadNormal, 0);
	//matBillboard[3] = glm::vec4(10.0f, 0.0f, 0.0f, 1.0f);


	float scale = 0.1f;

	glm::vec2 dims = textStr->dims();
	glm::vec3 center = 0.5f * glm::vec3(float(dims.x), float(dims.y), 0.0f);


	glm::mat4 matTransA = glm::translate(glm::mat4(1.0f), -center);
	glm::mat4 matTransB = glm::translate(glm::mat4(1.0f), center);
	glm::mat4 matScale = glm::scale(glm::mat4(1.0f), scale*glm::vec3(float(dims.x), float(dims.y), 1.0f));
	glm::mat4 matTransPos = glm::translate(glm::mat4(1.0f), pos);
	glm::mat4 matRot = glm::rotate(-angle, rotAxis);

	glm::mat4 matTransTmp = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.5f, 0.0f));

	m_shaderText->bind();
	
	glm::mat4 model = matTransPos;// *matScale;

	m_shaderText->setMatrix("matModel", model, GL_TRUE);
	m_shaderText->setMatrix("matView", view, GL_TRUE);
	m_shaderText->setMatrix("matProjection", projection, GL_TRUE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textStr->texId());
	m_shaderText->seti("tex", 0);

	m_vboQuad->render();

	m_shaderText->release();
}

TextString * TextRenderer::getTextString(const std::string &text, const std::string &font, int fontSize)
{		
	auto iter =	std::find_if(m_textStrings.begin(), m_textStrings.end(),
		[&](const TextString* s) 
	{ 
		return (s->fontSize() == fontSize &&
			    font.compare(s->font()) == 0 &&
			    text.compare(s->text()) == 0);
	});

	TextString * resPrt = nullptr;

	// first check if we already have a StringText object for the text
	if (iter == m_textStrings.end())
	{
		//init new text
		TextString *s = new TextString(text, fontSize, font);
		m_textStrings.push_back(s);

		resPrt = s;
		std::cout << "TextRenderer. Added new text string." << std::endl;
	}
	else
	{
		resPrt = *iter;
	}

	return resPrt;
}

void TextRenderer::renderSdf(TextString * text)
{
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glDisable(GL_DEPTH_TEST);


	//auto param = RenderContext::globalObjectParam();

	//glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	//glm::mat4 projection = glm::ortho(0.0f, float(param->windowWidth), 0.0f, float(param->windowHeight), -1.0f, 1.0f);

	//glm::vec2 dims = text->dims();
	//glm::vec2 pos = text->pos();

	//m_shaderTextSdf->bind();

	//glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, 0.0f));
	//glm::mat4 model = glm::scale(trans, glm::vec3(float(dims.x), float(dims.y), 1.0f));

	//m_shaderTextSdf->setMatrix("matModel", model, GL_TRUE);
	//m_shaderTextSdf->setMatrix("matView", view, GL_TRUE);
	//m_shaderTextSdf->setMatrix("matProjection", projection, GL_TRUE);

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, text->texId());
	//m_shaderTextSdf->seti("tex", 0);

	//m_vboQuad->render();

	//m_shaderTextSdf->release();
	//glEnable(GL_DEPTH_TEST);
}

TextRenderer* TextRenderer::instance()
{
	if (s_texRendererInstance == nullptr)
		s_texRendererInstance = new TextRenderer();

	return s_texRendererInstance;
}