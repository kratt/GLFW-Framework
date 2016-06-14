// Computer Graphics and Media Informatics
// University of Konstanz
// 2015

#ifndef OBJLOADER
#define OBJLOADER

#include "Common.h"

/*
-----------------------------------------------------------------------------
ObjLoader.h
-----------------------------------------------------------------------------

This class is responsible for creating a vertex buffer object (geometry
storage) from an *obj file.

Usage:
VertexBufferObjectAtribts vbo = 
				ObjLoader::createVertexBufferObject("path/to/objFile.obj");

-----------------------------------------------------------------------------
*/

class VertexBufferObjectAttribs;

class ObjLoader
{
public:

	static VertexBufferObjectAttribs* createVertexBufferObject(char* file);
};

#endif