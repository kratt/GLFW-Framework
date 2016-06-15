// Computer Graphics and Media Informatics
// University of Konstanz
// 2015

#include "src/Common.h"
#include "src/Application.h"
#include <iostream>

/*
-----------------------------------------------------------------------------
							main.cpp
-----------------------------------------------------------------------------

Entry point for the program. Initialized the libraries and sets up the OpenGL
context, input handlers, etc.

A short overview over the important classes within this project:

Application			-	Responsible for the rendering, class in which the 
						!!practical tasks should be solved!! .

Shader				-	Represents the vertex and fragment programs that are
						by default in the /Shader directory of the framework.

VertexBufferObjectAttribs	-	Geometry data, for example loaded from an
						*.obj file.
-----------------------------------------------------------------------------
*/

Application *app;

// global functions
void init();
void shutdown();

// event handler declarations
void glfw_onKey(GLFWwindow * window, GLint key, GLint scancode, GLint action, GLint mods);
void glfw_onMouseMove(GLFWwindow * window, GLdouble x, GLdouble y);
void glfw_onMouse(GLFWwindow * window, GLint button, GLint action, GLint mods);
void glfw_onScroll(GLFWwindow * window, GLdouble xo, GLdouble yo);
void glfw_onResize(GLFWwindow * window, GLint width, GLint height);

GLFWwindow*			g_glfwWindow		= NULL;
glm::vec2			g_oldMouse			= glm::vec2(0, 0);
GLboolean			g_leftButton		= GL_FALSE;
GLboolean			g_rightButton		= GL_FALSE;
const GLint			g_width				= 1024;
const GLint			g_height			= 768;
const std::string	g_projectName		= "framework";


/**
	Error callback - GLFW calls this on errors.
*/
void onError(GLint errorCode, const GLchar* errorMessage)
{
	std::cerr << "GLWindow::ERROR:\t" << errorMessage << std::endl;
	std::cerr << "press continue to exit";
	std::cin.get();
	exit(EXIT_FAILURE);
}

/** 
	Initialize GLFW, glew and OpenGL. Creates a window.
 */
void init()
{    
	if(!glfwInit())
		exit( EXIT_FAILURE );

	// initialize GLFW
	GLboolean error = glfwInit();
	if (!error)
	{
		std::cerr << "could not init GLFW:" << std::endl;
		std::cerr << "press enter to exit";
		std::cin.get();
		exit(EXIT_FAILURE);
	}

	// set depths
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_STENCIL_BITS, 8);
	glfwWindowHint(GLFW_DEPTH_BITS, 16);
	glfwWindowHint(GLFW_SAMPLES, 4);

	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); // allow the window to be resized

	OpenGLVersion maxVersion = Common::detectMaximumOpenGlVersion();
    
    glfwSetErrorCallback(onError);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, maxVersion.major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, maxVersion.minor);

	Common::setupContextHints(maxVersion);

	glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// create window
    g_glfwWindow = glfwCreateWindow(g_width, g_height, g_projectName.c_str(), NULL, NULL);
    

	// set callback functions
	glfwSetKeyCallback(g_glfwWindow, glfw_onKey);
	glfwSetWindowSizeCallback(g_glfwWindow, glfw_onResize);
	glfwSetMouseButtonCallback(g_glfwWindow, glfw_onMouse);
	glfwSetCursorPosCallback(g_glfwWindow, glfw_onMouseMove);
	glfwSetScrollCallback(g_glfwWindow, glfw_onScroll);

	// set current gl context
	glfwMakeContextCurrent(g_glfwWindow);

	// initialize GLEW only if included
	#ifdef __GLEW_H__
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cerr << "Error: " << glewGetErrorString(err);
		std::cerr << "press enter to exit";
		std::cin.get();
		exit(EXIT_FAILURE);
	}
	#endif

	GL_ASSERT_RESET();
	std::cout << "using OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	/*
	Set up default VAO 
	*/
	GLuint vaoID;
	GL_ASSERT( glGenVertexArrays(1, &vaoID) );
	GL_ASSERT( glBindVertexArray(vaoID) );

	GL_ASSERT( glEnable(GL_DEPTH_TEST) );
}

/**
	Keyboard input handler.
*/
void glfw_onKey(GLFWwindow * window, GLint key, GLint scancode, GLint action, GLint mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		shutdown();
	}
	else
	{
		app->onKey(key, scancode, action, mods);
	}
}

/**
	Mouse move handler.	
*/
void glfw_onMouseMove(GLFWwindow * window, GLdouble x, GLdouble y)
{
	glm::vec2 mouse((GLfloat)x, (GLfloat)y);
	glm::vec2 d = mouse - g_oldMouse;

	if (g_leftButton)
	{
		app->m_rotate.y += (0.1f * d.x);
		app->m_rotate.x += (0.1f * d.y);
	}

	g_oldMouse = mouse;
}

/**
	Mouse click handler.
*/
void glfw_onMouse(GLFWwindow * window, GLint button, GLint action, GLint mods)
{
	if(button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if(action == GLFW_PRESS)
			g_leftButton = GL_TRUE;
		else
			g_leftButton = GL_FALSE;
	}

	if(button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		if(action == GLFW_PRESS)
			g_rightButton = GL_TRUE;
		else
			g_rightButton = GL_FALSE;
	}
}

/**
	Mouse wheel handler.
*/
void glfw_onScroll(GLFWwindow * window, GLdouble xo, GLdouble yo)
{
	GLfloat delta = app->m_zoom * 0.1f;

	if (yo < 0)
	{
		app->m_zoom += delta;
	}
	else
	{
		app->m_zoom -= delta;
	}
}

/**
	Window resize handler.
*/
void glfw_onResize(GLFWwindow * window, GLint width, GLint height)
{
    glfwGetFramebufferSize(window, &app->m_framebufferWidth, &app->m_framebufferHeight);
	app->m_width = width;
	app->m_height = height;
}

/**
	Exit method that closes program.
*/
void shutdown()
{
	exit(EXIT_SUCCESS);
}

int main(int argc, const char* argv[])
{
	// initialize GLFW
	init();

	// create App
	app = new Application(g_width, g_height);
	app->m_zoom = 10.0f;
	app->m_rotate = glm::vec2(0.0f, 20.0f);
	app->m_showMesh = true;

	// initialize App
	app->init();

	// render loop
	while (!glfwWindowShouldClose(g_glfwWindow))
	{
        GL_ASSERT_RESET();
        glfwGetFramebufferSize(g_glfwWindow, &app->m_framebufferWidth, &app->m_framebufferHeight);
		app->render();

		glfwPollEvents();
		glfwSwapBuffers(g_glfwWindow);
	}

	shutdown();
	glfwTerminate();
    
    return 0;
}
