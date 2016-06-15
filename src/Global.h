// Computer Graphics and Media Informatics
// University of Konstanz
// 2015

#ifndef GLOBAL
#define GLOBAL


#ifdef __APPLE__
//tell glfw to include the gl-ext and gl-core 
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLEXT
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include "Assert.h"
#include <limits>

#define VERTEX_POSITION 0
#define VERTEX_NORMAL   1
#define VERTEX_COLOR    2
#define VERTEX_TEXTURE  3

const GLfloat math_pi       = 3.1415926536f;
const GLfloat math_radians  = math_pi / 180.0f;
const GLfloat math_degrees  = 180.0f / math_pi;
const GLfloat math_epsilon  = 1e-5f;
const GLfloat math_maxfloat = (std::numeric_limits<GLfloat>::max)();
const GLfloat math_minfloat = -(std::numeric_limits<GLfloat>::max)();

#endif