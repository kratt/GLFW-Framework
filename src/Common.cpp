// Computer Graphics and Media Informatics
// University of Konstanz
// 2015

#include "Common.h"
#include "VertexBufferObjectAttribs.h"

#include <iostream>

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

	vendor = (char*)glGetString(GL_VENDOR);
	renderer = (char*)glGetString(GL_RENDERER);
	version = (char*) glGetString(GL_VERSION);
	extentions = (char*) glGetString(GL_EXTENSIONS);

	printf("Vendor: %s\n", vendor);
	printf("Renderer: %s\n", renderer);
	printf("Version: %s\n", version);
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

void Common::getCameraFrame(const Transform & trans, glm::vec3 & dir, glm::vec3 & up, glm::vec3 & right, glm::vec3 & pos)
{
	glm::mat4 view = trans.view;

	up = glm::normalize(glm::vec3(view[0][1], view[1][1], view[2][1]));
	right = glm::normalize(glm::vec3(view[0][0], view[1][0], view[2][0]));
	dir = glm::normalize(glm::cross(up, right));

	pos.x = -(view[0][0] * view[0][3] + view[1][0] * view[1][3] + view[2][0] * view[2][3]);
	pos.y = -(view[0][1] * view[0][3] + view[1][1] * view[1][3] + view[2][1] * view[2][3]);
	pos.z = -(view[0][2] * view[0][3] + view[1][2] * view[1][3] + view[2][2] * view[2][3]);
}


double Common::hermiteInterpolation(double y0, double y1, double y2, double y3, double mu, double tension, double bias)
{
	double m0, m1, mu2, mu3;
	double a0, a1, a2, a3;

	mu2 = mu * mu;
	mu3 = mu2 * mu;
	m0 = (y1 - y0)*(1 + bias)*(1 - tension) / 2;
	m0 += (y2 - y1)*(1 - bias)*(1 - tension) / 2;
	m1 = (y2 - y1)*(1 + bias)*(1 - tension) / 2;
	m1 += (y3 - y2)*(1 - bias)*(1 - tension) / 2;
	a0 = 2 * mu3 - 3 * mu2 + 1;
	a1 = mu3 - 2 * mu2 + mu;
	a2 = mu3 - mu2;
	a3 = -2 * mu3 + 3 * mu2;

	return(a0*y1 + a1*m0 + a2*m1 + a3*y2);
}
