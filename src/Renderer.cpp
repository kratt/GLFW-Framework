#include "Renderer.h"
//#include "GUI.h"
//#include "CameraManager.h"
//#include "FrameBufferObjectMultisample.h"
#include "FrameBufferObject.h"
#include "Shader.h"
#include "VertexBufferObjectAttribs.h"
#include "Scene.h"
#include "Mesh.h"
#include "Light.h"
#include "Texture.h"
#include "RenderContext.h"
#include "Common.h"
#include "RenderUtils.h"
#include "TextRenderer.h"
#include "PngLoader.h"
#include "TextString.h"

Renderer::Renderer(Scene *scene, CameraManager *camManager)
: m_scene(scene),
  m_cameraManager(camManager),
  m_bgColor(0.1f, 0.1f, 0.1f, 1.0f),
  m_width(0),
  m_height(0),
  m_samples(16),
  m_bgMode(0),
  m_vboQuad(nullptr)
{
    init();
}

Renderer::~Renderer()
{
}

void Renderer::init()
{
	m_shaderQuad = new Shader("../Shader/Quad.vert.glsl", "../Shader/Quad.frag.glsl");
	m_shaderQuad->bindAttribLocations();
}

void Renderer::render()
{
	auto param = RenderContext::globalObjectParam();

    if(param->applyShadow)
    {
		m_scene->m_light->setLightView();
        m_scene->m_light->renderLightView(); 
    }    

    renderScene();
}

void Renderer::renderScene()
{
	auto param = RenderContext::globalObjectParam();

    glViewport(0, 0, m_width, m_height);
    glClearColor(m_bgColor.x, m_bgColor.y, m_bgColor.z, m_bgColor.w);    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     

    glEnable(GL_MULTISAMPLE);        

   if(param->polygonMode == 1)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
      m_scene->renderObjects();
	  m_scene->renderWorld();
	
	  glDisable(GL_CULL_FACE);


	  TextRenderer::instance()->render("void Renderer::renderfghQuad()", glm::vec2(100, 100), 24);
	  
	  //TextRenderer::instance()->render("void Renderer::renderfghQuad()", glm::vec3(10.0f), glm::vec2(5, 0));
	//   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::renderQuad()
{
	auto param = RenderContext::globalObjectParam();
	auto trans = RenderContext::transform();

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 projection = glm::ortho(0.0f, float(m_width), 0.0f, float(m_height), -1.0f, 1.0f);

	glDisable(GL_DEPTH_TEST);

	glViewport(0, 0, m_width, m_height);
	glClearColor(m_bgColor.x, m_bgColor.y, m_bgColor.z, m_bgColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_MULTISAMPLE);

	//if(param->polygonMode == 1)
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//else
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	m_shaderQuad->bind();

	m_shaderQuad->setMatrix("matModel", model, GL_TRUE);
	m_shaderQuad->setMatrix("matView", view, GL_TRUE);
	m_shaderQuad->setMatrix("matProjection", projection, GL_TRUE);

	m_vboQuad->render();

	m_shaderQuad->release();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::resize(int width, int height)
{
    m_width = width;
    m_height = height;    

	delete m_vboQuad;
	m_vboQuad = Mesh::quad(0, 0, m_width, m_height);
}

void Renderer::toggleBGColor()
{
    m_bgMode ++;
    if(m_bgMode > 2)
        m_bgMode = 0;

    if(m_bgMode == 0)
	{
        m_bgColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
		//m_gui->setFontColor(glm::vec4(0.9f, 0.9f, 0.9f, 1.0f));
	}
   
    if(m_bgMode == 1)
	{
        m_bgColor = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
		//m_gui->setFontColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	}

    if(m_bgMode == 2)
	{
        m_bgColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		//m_gui->setFontColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	}
}

