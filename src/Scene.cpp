#include "Scene.h"
#include "NiceGrid.h"
#include "Light.h"
#include "Shader.h"
#include "VertexBufferObjectAttribs.h"
#include "Mesh.h"
#include "CameraManager.h"
#include "Object.h"
#include "RenderContext.h"

Scene::Scene(CameraManager *camManager)
: m_cameraManager(camManager),
  m_activeIdx(-1)
{
    init();   
}

Scene::~Scene()
{
}

void Scene::init()
{
    m_light = new Light(this, glm::vec3(0.0f, 20.0f, 0.1f));
	m_niceGrid = new NiceGrid(100.0f, 40.0f);  

    initShaders();
}

void Scene::renderWorld()
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);

	m_niceGrid->render(); 
    m_cameraManager->renderCameras();

	glPopClientAttrib();
    glPopAttrib();
}

void Scene::renderObjects()
{
	auto param = RenderContext::globalObjectParam();
	auto trans = RenderContext::transform();

    glm::mat4 projection = trans->projection;
	glm::mat4 view = trans->view;
	glm::mat4 model = glm::mat4(1.0f);

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);


  //  m_shaderNormal->bind();
  //      m_shaderNormal->set3f("lightPos", param->lightPos);

  //      m_shaderNormal->setMatrix("matProjection", projection, GL_FALSE);
		//m_shaderNormal->setMatrix("matView", view, GL_FALSE);

  //      model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.0f));
		//m_shaderNormal->setMatrix("matModel", model, GL_FALSE);
  //      
  //      m_vbo->render();    

  //  m_shaderNormal->release();

	glPopClientAttrib();
    glPopAttrib();
}

void Scene::renderObjectsDepth()
{
	auto param = RenderContext::globalObjectParam();
	auto trans = RenderContext::transform();

    glm::mat4 projection = trans->lightProjection;
    glm::mat4 view = trans->lightView;
	glm::mat4 model = glm::mat4(1.0f);

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);


    //m_shaderDepth->bind();

    //    m_shaderDepth->setMatrix("matProjection", projection, GL_TRUE);
    //    m_shaderDepth->setMatrix("matView", view, GL_TRUE);

    //    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.0f));
    //    m_shaderDepth->setMatrix("matModel", model, GL_TRUE); 
    //    
    //    m_vbo->render();    

    //m_shaderDepth->release();

	glPopClientAttrib();
    glPopAttrib();
}
 
void Scene::update(float delta)
{
   // m_light->update(delta);
}

void Scene::initShaders()
{
    m_shaderNormal = new Shader("../Shader/DefaultLight.vert.glsl", "../Shader/DefaultLight.frag.glsl");
    m_shaderNormal->bindAttribLocations();

    m_shaderDepth = new Shader("../Shader/Default.vert.glsl", "../Shader/DefaultDepth.frag.glsl");
    m_shaderDepth->bindAttribLocations();
}

void Scene::select(int sw, int sh, int mx, int my)
{

}

void Scene::move(int x, int y)
{

}

void Scene::resetSelection()
{

}