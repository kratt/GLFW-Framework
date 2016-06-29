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

	m_vbo = Mesh::sphere(2.0f, 6, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

	Object *sphere = new Object("../Data/Objs/sphere.obj", glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(1, 1, 1));
	m_objects.push_back(sphere);
}

void Scene::renderWorld()
{
	m_niceGrid->render(); 
	m_light->render();
   // m_cameraManager->renderCameras();
}

void Scene::renderObjects()
{
	auto param = RenderContext::globalObjectParam();
	auto trans = RenderContext::transform();

 //   glm::mat4 projection = trans->projection;
	//glm::mat4 view = trans->view;
	//glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.0f));


 //   m_shaderNormal->bind();
 //       m_shaderNormal->set3f("lightPos", param->lightPos);

 //       m_shaderNormal->setMatrix("matProjection", projection, GL_FALSE);
	//	m_shaderNormal->setMatrix("matView", view, GL_FALSE);
	//	m_shaderNormal->setMatrix("matModel", model, GL_FALSE);
 //       
 //       m_vbo->render();    

 //   m_shaderNormal->release();



	for (int i = 0; i < m_objects.size(); ++i)
	{
		m_objects[i]->render();
	}
	
}

void Scene::renderObjectsDepth()
{
	//auto param = RenderContext::globalObjectParam();
	//auto trans = RenderContext::transform();

 //   glm::mat4 projection = trans->lightProjection;
 //   glm::mat4 view = trans->lightView;
	//glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.0f));

 //   m_shaderDepth->bind();

 //       m_shaderDepth->setMatrix("matProjection", projection, GL_FALSE);
 //       m_shaderDepth->setMatrix("matView", view, GL_FALSE);
 //       m_shaderDepth->setMatrix("matModel", model, GL_FALSE);
 //       
 //       m_vbo->render();    

 //   m_shaderDepth->release();

	for (int i = 0; i < m_objects.size(); ++i)
	{
		m_objects[i]->renderDepth();
	}
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