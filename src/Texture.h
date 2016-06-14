//Author: Thomas Lindemeier
//Date: 09.04.2014
// http://nehe.gamedev.net/tutorial/loading_compressed_and_uncompressed_tga's/22001/

#ifndef MY_TEXTURE_H
#define MY_TEXTURE_H

#include "Global.h"

struct Texture
{
	// Image Data (Up To 32 Bits)
	GLubyte	* imageData;
	// Image Color Depth In Bits Per Pixel
	GLuint	bpp;				
	// Image Width
	GLuint	width;	
	// Image Height
	GLuint	height;		
	// Texture ID Used To Select A Texture
	GLuint	texID;	
	// Image Type (GL_RGB, GL_RGBA)
	GLuint	type;											
};


#endif // MY_TEXTURE_H