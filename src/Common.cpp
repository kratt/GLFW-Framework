// Computer Graphics and Media Informatics
// University of Konstanz
// 2015

#include "Common.h"
#include "VertexBufferObjectAttribs.h"

/**
	Returns a random float value between low and high.
*/
float frand(float low, float high)
{
	float result = (rand() / (static_cast<float>(RAND_MAX) + 1.0f)) * (high - low) + low;
	return result;
}

/**
	Prints OpenGL vendor, renderer, as well as version.
*/
void checkOpenGL()
{
	char *vendor = NULL;
	char *renderer = NULL;
	char *version = NULL;
	char *extentions = NULL;

	vendor = (char*)GL_ASSERT( glGetString(GL_VENDOR) );
	renderer = (char*)GL_ASSERT( glGetString(GL_RENDERER) );
	version = (char*)GL_ASSERT( glGetString(GL_VERSION) );
	extentions = (char*)GL_ASSERT( glGetString(GL_EXTENSIONS) );

	printf("Vendor: %s\n", vendor);
	printf("Renderer: %s\n", renderer);
	printf("Version: %s\n", version);
}


/**
	Returns the camera position from a given ModelView-Matrix.
*/
vec3 getCamPosFromModelView(const Transform &trans)
{
	mat4 m = trans.view;
	vec3 c;

	c.x = -(m.a11 * m.a43 + m.a21 * m.a24 + m.a31 * m.a34);
	c.y = -(m.a12 * m.a43 + m.a22 * m.a24 + m.a32 * m.a34);
	c.z = -(m.a13 * m.a43 + m.a23 * m.a24 + m.a33 * m.a34);

	return c;
}

/**
	OpenGL context setup.
*/
void Common::setupContextHints(const OpenGLVersion &version) {

	if ((version.major == 3 && version.minor > 1) || version.major >= 4) {
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	}
	else {
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
	}
}

/**
	Tests the supported OpenGL version.
*/
bool Common::supportsGlVersion(int major, int minor) {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);


	setupContextHints(OpenGLVersion(major, minor));

	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);

	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow *window = glfwCreateWindow(100, 100, "context test", NULL, NULL);
	if (!window)
	{
		return false;
	}
	else {
		glfwDestroyWindow(window);
		return true;;
	}
}

/**
	Detects the maximum supported OpenGL version.
*/
OpenGLVersion Common::detectMaximumOpenGlVersion() {
	for (int major = 4; major >= 3; major--) {
		for (int minor = 5; minor >= 0; minor--) {
			if (supportsGlVersion(major, minor)) {
				return OpenGLVersion( major, minor );
			}
		}
	}
	return OpenGLVersion( 0, 0 );

}

/**
	Setup OpenGL extensions.
*/
void Common::setupOpenGLExtensions() {
	std::cout << "Setup GLEW ... ";
#ifdef __GLEW_H__
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "failed: " << glewGetErrorString(err) << std::endl;
		std::cout << "press enter to exit";
		std::cin.get();
		exit(EXIT_FAILURE);
	}
	else {
		std::cout << "success." << std::endl;
	}
#else
	std::cout << "not required." << std::endl;
#endif
}
