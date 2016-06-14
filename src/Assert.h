// Computer Graphics and Media Informatics
// University of Konstanz
// 2015

#define __DEBUG__MODE__ 0


#ifdef __DEBUG__MODE__
#include <iostream>
#include <string>

#ifndef GL_STACK_OVERFLOW
#define GL_STACK_OVERFLOW 0x0503
#endif


#ifndef GL_STACK_UNDERFLOW
#define GL_STACK_UNDERFLOW 0x0504
#endif

inline std::string errorNoStoString(GLenum errorNo) {

	switch (errorNo) {
	case GL_NO_ERROR:
		return "GL_NO_ERROR";
	case GL_INVALID_ENUM:
		return "GL_INVALID_ENUM";
	case GL_INVALID_VALUE:
		return "GL_INVALID_VALUE";
	case GL_INVALID_OPERATION:
		return "GL_INVALID_OPERATION";
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		return "GL_INVALID_FRAMEBUFFER_OPERATION";
	case GL_OUT_OF_MEMORY:
		return "GL_OUT_OF_MEMORY";
	default:
		throw "unknown error number";
	}


}

inline void printAssert(std::string cmd, int line, std::string file) {
	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		std::cout << "Error: " << errorNoStoString(err) << " for " << cmd << " " << std::endl;
		std::cout << "       Line:  " << line << " file: " << file << std::endl << std::endl;
	}
}

inline void glAssertReset() {
	glGetError();
}


#define GL_ASSERT(EXP) \
		 EXP; \
		 printAssert(#EXP, __LINE__, __FILE__ );

#define GL_ASSERT_RESET()	glAssertReset();


#else

#define GL_ASSERT( EXP ) EXP
#define GL_ASSERT_RESET()	
#endif