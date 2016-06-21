// Computer Graphics and Media Informatics
// University of Konstanz
// 2015

#include "src/Common.h"
#include "src/GLWindow.h"
#include "src/debug.h"
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

GLWindow *app;

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
const GLint			g_width				= 1280;
const GLint			g_height			= 720;
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

//#ifndef NDEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
//#endif


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

	//GLint flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	//if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	//{
	//	setupDebug();
	//}

	std::cout << "using OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	/*
	Set up default VAO 
	*/
	GLuint vaoID;
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	glEnable(GL_DEPTH_TEST);
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
		if(action == GLFW_PRESS)
			app->keyPressEvent(key, scancode, action, mods);
		if(action == GLFW_RELEASE)
			app->keyReleaseEvent(key, scancode, action, mods);
	}
}

/**
	Mouse move handler.	
*/
void glfw_onMouseMove(GLFWwindow * window, GLdouble x, GLdouble y)
{
	app->mouseMoveEvent(x, y);
}

/**
	Mouse click handler.
*/
void glfw_onMouse(GLFWwindow * window, GLint button, GLint action, GLint mods)
{
	app->mousePressEvent(button, action, mods);
}

/**
	Mouse wheel handler.
*/
void glfw_onScroll(GLFWwindow * window, GLdouble xo, GLdouble yo)
{
	app->mouseWheelEvent(xo, yo);
}

/**
	Window resize handler.
*/
void glfw_onResize(GLFWwindow * window, GLint width, GLint height)
{
	app->resize(width, height);
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
	app = new GLWindow(g_width, g_height);

	// render loop
	while (!glfwWindowShouldClose(g_glfwWindow))
	{
		app->render();

		glfwPollEvents();
		glfwSwapBuffers(g_glfwWindow);
	}

	shutdown();
	glfwTerminate();
    
    return 0;
}
