#include "GLWindow.h"
#include "Shader.h"
#include "ObjLoader.h"
#include "VertexBufferObjectAttribs.h"
#include "Texture.h"
#include "RenderContext.h"
#include "Renderer.h"
#include "CameraManager.h"
#include "Scene.h"
#include "Light.h"
#include "GUI.h"
#include "opengl_utils.h"
#include "system_utils.h"
#include "TextRenderer.h"
#include "RenderUtils.h"


#include <iostream>


GLWindow::GLWindow(int width, int height) :
	m_width(width),
	m_height(height),
	m_ctrlPressed(false),
	m_altPressed(false),
	m_shiftPressed(false),
	m_noOtherKey(true),
	m_leftButton(false),
	m_rightButton(false),
	m_numFrames(0)
{
	init();
	initParams();

	m_lastTime = glfwGetTime();

	this->resize(m_width, m_height);
}

GLWindow::~GLWindow()
{

}

void GLWindow::init()
{
	m_cameraManager = new CameraManager();
	m_scene = new Scene(m_cameraManager);
	m_gui = new GUI();
	m_renderer = new Renderer(m_scene, m_cameraManager);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);

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

void GLWindow::render()
{
	auto param = RenderContext::globalObjectParam();
	auto trans = RenderContext::transform();

	m_cameraManager->currentPerspective(*trans);
	m_cameraManager->currentCamParams();

	param->windowWidth = this->m_width;
	param->windowHeight = this->m_height;
	param->lightPos = m_scene->m_light->position();

	double currentTime = glfwGetTime();
	m_numFrames++;
	if (currentTime - m_lastTime >= 1.0) { 
		param->mspf = 1000.0 / double (m_numFrames);
		param->fps = m_numFrames;
		m_numFrames = 0;
		m_lastTime += 1.0;
	}

	m_renderer->render();
	m_gui->render();
}

void GLWindow::keyPressEvent(GLint key, GLint scancode, GLint action, GLint mods)
{
	auto param = RenderContext::globalObjectParam();

	m_cameraManager->onKeyPress(key);
	
	switch (key)
	{
		case GLFW_KEY_SPACE:
			Common::loop(param->polygonMode, 0, 1, 1);
			break;
		case GLFW_KEY_PAGE_UP:
			m_cameraManager->increaseSpeed();
			break;
		case GLFW_KEY_PAGE_DOWN:
			m_cameraManager->decreaseSpeed();
			break;
		case GLFW_KEY_F2:
			m_cameraManager->toggleCam();
			break;
		case GLFW_KEY_F3:
			if (m_ctrlPressed)
				utils::save_depthbuffer("./../" + utils::time_string() + ".png");
			else
				utils::save_framebuffer("./../" + utils::time_string() + ".png");
			break;
		case GLFW_KEY_F4:
			m_renderer->toggleBGColor();
			break;
		case GLFW_KEY_F5:
			if (m_ctrlPressed)
				m_cameraManager->toggleFrameset();
			break;
		case GLFW_KEY_F6:
			if (m_ctrlPressed)
				m_cameraManager->addFrame();
			break;
		case GLFW_KEY_F7:
			if (m_ctrlPressed)
				m_cameraManager->clearFrameset();
			break;
		case GLFW_KEY_F8:
			if (m_ctrlPressed)
				m_cameraManager->saveFrameset();
			break;
		case GLFW_KEY_I:
			m_cameraManager->toggleInterpolation();
			break;
	}

	switch (key)
	{
		case GLFW_KEY_LEFT_CONTROL:
		case GLFW_KEY_RIGHT_CONTROL:
			m_ctrlPressed = true;
			m_noOtherKey = false;
			break;
		case GLFW_KEY_LEFT_ALT:
		case GLFW_KEY_RIGHT_ALT:
			m_altPressed = true;
			m_noOtherKey = false;
			break;
		case GLFW_KEY_LEFT_SHIFT:
		case GLFW_KEY_RIGHT_SHIFT:
			m_shiftPressed = true;
			m_noOtherKey = false;
			break;
	}
}

void GLWindow::keyReleaseEvent(GLint key, GLint scancode, GLint action, GLint mods)
{
	m_cameraManager->onKeyRelease(key);

	switch (key)
	{
		case GLFW_KEY_LEFT_CONTROL:
		case  GLFW_KEY_RIGHT_CONTROL:
			m_ctrlPressed = false;
			m_noOtherKey = true;
			break;
		case GLFW_KEY_LEFT_ALT:
		case GLFW_KEY_RIGHT_ALT:
			m_altPressed = false;
			m_noOtherKey = true;
			break;
		case GLFW_KEY_LEFT_SHIFT:
		case GLFW_KEY_RIGHT_SHIFT:
			m_shiftPressed = false;
			m_noOtherKey = true;
			break;
		default:
			break;
	}
}

void GLWindow::mouseMoveEvent(GLdouble x, GLdouble y)
{
	glm::vec2 newMouse(x, y);
	glm::vec2 diff = newMouse - m_mouse;

	m_gui->onMouseMove(x, y);

	if (!m_altPressed && !m_ctrlPressed)
	{
		if (m_leftButton)
		{
			m_cameraManager->onMouseMove(diff.x, diff.y, 0);
		}
		else if (m_rightButton)
		{
			m_cameraManager->onMouseMove(diff.x, diff.y, 1);
		}
	}

	if (m_leftButton && m_altPressed)
	{
	/*	if (m_ctrlPressed)
			m_scene->m_light->recordPath(true);
		else
			m_scene->m_light->recordPath(false);*/

		m_scene->m_light->move(m_cameraManager, diff.x*0.1, diff.y*0.1);
	}

	if (m_leftButton && m_ctrlPressed)
	{
		m_scene->move(diff.x, diff.y);
	}

	m_mouse = newMouse;
}

void GLWindow::mousePressEvent(GLint button, GLint action, GLint mods)
{
	// mouse press
	if (action == GLFW_PRESS)
	{
		if (!m_gui->onMouseClick(m_mouse.x, m_mouse.y))
		{
			if (!m_gui->onMouseClick(m_mouse.x, m_mouse.y))
			{
				if (button == GLFW_MOUSE_BUTTON_LEFT)
					m_leftButton = GL_TRUE;

				if (button == GLFW_MOUSE_BUTTON_RIGHT)
					m_rightButton = GL_TRUE;
			}
		}
	}

	if (action == GLFW_RELEASE)
	{
		m_gui->onMouseRelease();

		if (button == GLFW_MOUSE_BUTTON_LEFT)
			m_leftButton = GL_FALSE;

		if (button == GLFW_MOUSE_BUTTON_RIGHT)
			m_rightButton = GL_FALSE;
	}


	if (m_ctrlPressed)
	{
		m_scene->resetSelection();
		//m_scene->select(this->width(), this->height(), event->x(), event->y());
	}
}

void GLWindow::mouseWheelEvent(GLdouble xo, GLdouble yo)
{
	if (!m_altPressed && !m_ctrlPressed)
	{
		m_cameraManager->onMouseWheel(int(yo));
	}

	if (m_altPressed)
	{ 
		glm::vec3 lPos = m_scene->m_light->position();
		float delta = lPos.y * 0.1;

		if (yo > 0)
			m_scene->m_light->setPosition(glm::vec3(lPos.x, lPos.y + delta, lPos.z));
		else
			m_scene->m_light->setPosition(glm::vec3(lPos.x, lPos.y - delta, lPos.z));
	}
}

void GLWindow::resize(int w, int h)
{
	m_width = w;
	m_height = h;

	m_renderer->resize(w, h);
	m_cameraManager->resize(w, h);
	m_gui->resize(w, h);
}
