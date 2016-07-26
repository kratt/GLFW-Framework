#include "RenderUtils.h"
#include "Shader.h"
#include "VertexBufferObject.h"
#include "Mesh.h"
#include "RenderContext.h"

#include <glm/vec2.hpp>

RenderUtils* RenderUtils::s_renderUtilInstance = nullptr;


RenderUtils::RenderUtils()
: m_shaderTex2D(nullptr),
  m_vboQuad(nullptr)
{
	init();
}

RenderUtils::~RenderUtils()
{

}

void RenderUtils::init()
{
	m_shaderTex2D = new Shader("../Shader/Texture2D.vert.glsl", "../Shader/Texture2D.frag.glsl");
	m_shaderTex2D->bindAttribLocations();

	// scale quad according to needs later
	m_vboQuad = Mesh::quad(0, 0, 1, 1);
}


void RenderUtils::renderTexture(GLuint texId, int x, int y, int width, int height)
{
	auto param = RenderContext::globalObjectParam();

	glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(float(param->windowWidth), float(param->windowHeight), 1.0f));
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 projection = glm::ortho(0.0f, float(param->windowWidth), 0.0f, float(param->windowHeight), -1.0f, 1.0f);

	glDisable(GL_DEPTH_TEST);

	m_shaderTex2D->bind();

	m_shaderTex2D->setMatrix("matModel", model, GL_TRUE);
	m_shaderTex2D->setMatrix("matView", view, GL_TRUE);
	m_shaderTex2D->setMatrix("matProjection", projection, GL_TRUE);

	m_shaderTex2D->set2f("windowSize", param->windowWidth, param->windowHeight);
	m_shaderTex2D->set2f("texSize", width, height);
	m_shaderTex2D->set2f("texStartPos", x, y);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texId);
	m_shaderTex2D->seti("tex", 0);
	
	m_vboQuad->render();

	m_shaderTex2D->release();
	glEnable(GL_DEPTH_TEST);
}

RenderUtils* RenderUtils::instance()
{
	if (s_renderUtilInstance == nullptr)
		s_renderUtilInstance = new RenderUtils();

	return s_renderUtilInstance;
}