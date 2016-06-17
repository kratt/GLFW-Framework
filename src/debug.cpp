#include <iostream>

#include "Global.h"
// void APIENTRY glDebugMessageCallback (GLDEBUGPROC callback, const void
// *userParam);
// GLuint APIENTRY glGetDebugMessageLog (GLuint count, GLsizei bufSize, GLenum
// *sources, GLenum *types, GLuint *ids, GLenum *severities, GLsizei *lengths,
// GLchar *messageLog);

void APIENTRY openglCallbackFunction(GLenum source, GLenum type, GLuint id,
                                     GLenum severity, GLsizei length,
                                     const GLchar* message,
                                     const void* userParam) { //const ?


  std::cout << "\n\n---------------------opengl-callback-start------------"
            << std::endl;
  std::cout << "message: " << message << std::endl;
  std::cout << "type: ";
  switch (type) {
    case GL_DEBUG_TYPE_ERROR:
      std::cout << "ERROR";
      break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
      std::cout << "DEPRECATED_BEHAVIOR";
      break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
      std::cout << "UNDEFINED_BEHAVIOR";
      break;
    case GL_DEBUG_TYPE_PORTABILITY:
      std::cout << "PORTABILITY";
      break;
    case GL_DEBUG_TYPE_PERFORMANCE:
      std::cout << "PERFORMANCE";
      break;
    case GL_DEBUG_TYPE_OTHER:
      std::cout << "OTHER";
      break;
  }
  std::cout << std::endl;

  std::cout << "id: " << id << std::endl;
  std::cout << "severity: ";
  switch (severity) {
  case GL_DEBUG_SEVERITY_NOTIFICATION:
	  std::cout << "other";
		  break;
    case GL_DEBUG_SEVERITY_LOW:
      std::cout << "LOW";
      break;
    case GL_DEBUG_SEVERITY_MEDIUM:
      std::cout << "MEDIUM";
      break;
    case GL_DEBUG_SEVERITY_HIGH:
      std::cout << "HIGH";
      break;
  }
  std::cout << std::endl;
  std::cout << "---------------------opengl-callback-end--------------"
	  << std::endl << std::endl << std::endl;
}

void APIENTRY testDebug(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar *message,
	const void *userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
		case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
		case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
		case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type)
	{
		case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
		case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
		case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
		case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
		case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
		case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
		case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity)
	{
		case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
		case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
		case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << std::endl;
	std::cout << std::endl;
}

void setupDebug() {
//#ifndef NDEBUG
  if (glDebugMessageCallback) {
    std::cout << "Register OpenGL debug callback " << std::endl;
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(testDebug, nullptr);
    GLuint unusedIds = 0;
   //glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_HIGH, 0, nullptr, GL_TRUE);

   glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_ERROR,  GL_DEBUG_SEVERITY_MEDIUM, 0, nullptr, GL_TRUE);

  } else {
    std::cout << "glDebugMessageCallback not available" << std::endl;
  }

//#else
//  std::cout << "opengl debugging not enabled" << std::endl;
//#endif
}
