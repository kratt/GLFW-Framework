// Computer Graphics and Media Informatics
// University of Konstanz
// 2015

#include "Application.h"
#include "Shader.h"
#include "ObjLoader.h"
#include "VertexBufferObjectAttribs.h"
#include "Texture.h"
#include "TgaLoader.h"

/*
-----------------------------------------------------------------------------
							Application.cpp
-----------------------------------------------------------------------------

This class is responsible for the rendering. 
A short description of the methods:

render()		-	Render loop method that is called for each frame.
init()			-	Is called once when the program is started.
reloadShader()	-	Is called when 'F5' is pressed to reload the shaders.
onKey()			-	Handles keyboard inputs.


Note: The GL_ASSERT() function is simply used to catch possible errors
when calling OpenGL functions - this is for debug purposes and you do
not need to write it around every OpenGL call you make!

-----------------------------------------------------------------------------
*/

/**
	Constructor and destructor.
*/
Application::Application(int width, int height)	:
	m_width(width),
	m_height(height)
{

}

Application::~Application()
{

}


/**
	This method is called once. Use it to initialize shaders, vertex buffer objects, and elements that do not change during the render loop.
*/
void Application::init()
{
	// The shader constructor takes as argument the files in which the Vertex and the Fragment shader are defined.
	m_shader = new Shader("../Shader/Default33.vert.glsl", "../Shader/Default33.frag.glsl");
	// --> If this is not working for you, use the line below.
	//m_shader = new Shader("../Shader/Default31.vert.glsl", "../Shader/Default31.frag.glsl");

	// A vertex buffer object (which stores geometry) is loaded from an *.obj file.
	m_vbo = ObjLoader::createVertexBufferObject("../Data/box_long.obj");
}


/**
	This method is called when the key 'F5' is pressed. It can be used to reload the shader if changes have been made, without having to restart the whole program.
*/
void Application::reloadShader()
{
	delete m_shader;
	m_shader = new Shader("../Shader/Default33.vert.glsl", "../Shader/Default33.frag.glsl");
	// --> If this is not working for you, use the line below.
	//m_shader = new Shader("../Shader/Default31.vert.glsl", "../Shader/Default31.frag.glsl");
}



/**
	This method is called when a key is pressed / released. Use it to bind actions to keys.
*/
void Application::onKey(GLint key, GLint scancode, GLint action, GLint mods) {

	if (action == GLFW_PRESS)
	{
		// key 'M' toggles between rendering the mesh as a wire and rendering as solid
		if (key == 'M' && action == GLFW_PRESS)
		{
			m_showMesh = !m_showMesh;
		}
		// key 'F5' reloads shaders
		if (key == GLFW_KEY_F5 && action == GLFW_PRESS)
		{
			reloadShader();
		}
	}
}



/**
	The render loop. This is called for each frame.
*/
void Application::render()
{
	// Setting up the viewport of the window.
	glViewport(0, 0, m_framebufferWidth, m_framebufferHeight);

	// The rendering canvas is cleared with the clear color. Also the depth buffer is cleared.
	glClearColor(1.0f, 1.0f, 1.0f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// When pressing 'M', it is possible to toggle between a wireframe rendering mode and a solid rendering.
	if (!m_showMesh)
	{
		// Wireframe line rendering.
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		// 'Normal' solid rendering.
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}


	// The transformation matrices are set up which define the projection, the view frustum, and the transformation/scaling/rotation of the model.


	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)m_width / (GLfloat)m_height, 0.1f, 1000.0f);
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -m_zoom));
	view = glm::rotate(view, glm::radians(m_rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::rotate(view, glm::radians(m_rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4x4 model = glm::mat4x4(1.0f);


	// The shader is activated. The upcoming geometry will be passed through the Vertex and Fragment shaders.
	m_shader->bind();

		// Parameters can be transferred to the GPU as "uniform" variables (visible in all shaders).
		// Other types (similar for int):
		//	setf	->	1D float
		//	set2f	->	2D float
		//	set3f	->	3D float
		m_shader->setMatrix("matProjection", projection, GL_TRUE);
		m_shader->setMatrix("matView", view, GL_TRUE);
		m_shader->setMatrix("matModel", model, GL_TRUE);

		// The geometry is pushed through the rendering pipeline. Vertex shader is called for each vertex; fragment shader for each fragment (potential pixel).
		m_vbo->render();

	// The shader is deactivated.
	m_shader->release();
}
