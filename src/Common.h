// Computer Graphics and Media Informatics
// University of Konstanz
// 2015

#ifndef COMMON
#define COMMON

#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp> 
#include <glm/vec4.hpp> 
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/gtc/matrix_transform.hpp>

/*
-----------------------------------------------------------------------------
Common.h
-----------------------------------------------------------------------------

Contains some commonly used definitions, like VertexBufferObjects, TGAFILE,
as well as methods for checking the OpenGL versions. Also a few useful
utility functions.

-----------------------------------------------------------------------------
*/

class VertexBufferObjectAttribs;

using namespace std;

struct Transform
{
	glm::mat4x4 view;
	glm::mat4x4 projection;
};

struct TGAFILE
{
    unsigned char imageTypeCode;
    short int imageWidth;
    short int imageHeight;
    unsigned char bitCount;
    unsigned char *imageData;
};



class OpenGLVersion {
public:
	OpenGLVersion(int _major, int _minor) {
		this->major = _major;
		this->minor = _minor;	
	}
	int major;
	int minor;
};

struct Common {

	static bool supportsGlVersion(int major, int minor);

	static void setupContextHints(const OpenGLVersion &version);
	static OpenGLVersion detectMaximumOpenGlVersion();

	static void setupOpenGLExtensions();
};

float frand(float low, float high);
void checkOpenGL();

#endif
