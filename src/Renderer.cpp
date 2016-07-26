#include "Renderer.h"
#include "CameraManager.h"
#include "Shader.h"
#include "Scene.h"
#include "Light.h"
#include "RenderContext.h"


Renderer::Renderer(Scene *scene, CameraManager *camManager)
: m_scene(scene),
  m_cameraManager(camManager),
  m_bgColor(0.1f, 0.1f, 0.1f, 1.0f),
  m_width(0),
  m_height(0),
  m_bgMode(0)
{
}

Renderer::~Renderer()
{
}

void Renderer::render()
{
	auto param = RenderContext::globalObjectParam();

    if(param->updateShadow)
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

     glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::resize(int width, int height)
{
    m_width = width;
    m_height = height;    
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

