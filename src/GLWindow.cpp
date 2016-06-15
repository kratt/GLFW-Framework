// Computer Graphics and Media Informatics
// University of Konstanz
// 2015

#include "GLWindow.h"
#include "Shader.h"
#include "ObjLoader.h"
#include "VertexBufferObjectAttribs.h"
#include "Texture.h"
#include "TgaLoader.h"
#include "RenderContext.h"
#include "Renderer.h"
#include "CameraManager.h"
#include "Scene.h"



GLWindow::GLWindow(int width, int height)	:
	m_width(width),
	m_height(height)
{
	init();
	initParams();

	this->resize(m_width, m_height);
}

GLWindow::~GLWindow()
{

}

void GLWindow::init()
{
	m_cameraManager = new CameraManager();
	m_scene = new Scene(m_cameraManager);
	//m_gui = new GUI(m_cameraManager, m_scene);
	m_renderer = new Renderer(m_scene, m_cameraManager);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_SAMPLE_SHADING);
	glMinSampleShading(0.0f);
}

void GLWindow::initParams()
{
	auto param = RenderContext::globalObjectParam();

	param->lightPos = glm::vec3(0.0f, 0.0f, 0.0f);
	param->lightDir = glm::vec3(0.0f, 0.0f, 0.0f);
	param->camPos = glm::vec3(0.0f, 0.0f, 0.0f);
	param->applyShadow = true;
	param->gridRenderMode = 0;
	param->shadowIntensity = 0.0f;

	param->shadowMapID = 0;
	param->shadowMapBlurredID = 0;
	param->polygonMode = 0;

	param->windowWidth = m_width;
	param->windowHeight = m_height;

	param->renderMesh = false;
	param->renderObjects = true;

	param->clipPlaneGround = glm::vec4(0.0f, -1.0f, 0.0f, 4.0f);
	param->ncp = 0.0f;
	param->fcp = 0.0f;
	param->fov = 0.0f;

	param->polygonOffsetUnits = 0.0f;
	param->polygonOffsetFactor = 0.0f;
	param->depthRangeMax = 1.0f;
	param->depthRangeMin = 0.0f;
}

/**
	The render loop. This is called for each frame.
*/
void GLWindow::render()
{
	auto param = RenderContext::globalObjectParam();

	//m_cameraManager->currentPerspective(m_trans);
	//m_cameraManager->currentCamParams();

	param->windowWidth = this->m_width;
	param->windowHeight = this->m_height;
	//param->lightPos = m_scene->m_light->position();

	m_renderer->render();

	//DWORD time = GetTickCount();
	//float delta = time - m_oldTime;
	//if (delta > 25)
	//{
	//	m_scene->update(1.0f / delta);
	//	m_oldTime = time;
	//}

	//this->update();
}

void GLWindow::resize(int w, int h)
{
	m_width = w;
	m_height = h;

	m_renderer->resize(w, h);
	//m_cameraManager->resize(w, h);
	//m_gui->resize(w, h);
}
