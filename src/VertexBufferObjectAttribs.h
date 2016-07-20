// Computer Graphics and Media Informatics
// University of Konstanz
// 2015

#ifndef VERTEXBUFFEROBJECTATTRIBS
#define VERTEXBUFFEROBJECTATTRIBS

#include "Common.h"
#include "Global.h"

/*
-----------------------------------------------------------------------------
							VertexBufferObjectAttribs.h
-----------------------------------------------------------------------------

Stores geometry data (Vertex position, normals, color, texture coordinates,
as well as face definitions).
A short usage description:

1.	VertexBufferObjectAttribs vbo =
					ObjLoader::createVertexBufferObject(char* pathToFile);
	Loads a geometry file (*.obj) with the ObjLoader class.

2.	render()
	Renders the geometry. Before this call, a shader has to be bound with the
	the shaders bind() function! Basically:
	shader->bind();
		vbo->render();
	shader->release();

-----------------------------------------------------------------------------
*/

class VertexBufferObjectAttribs
{
public:
    struct DATA
    {
        GLfloat vx, vy, vz, vw; // Position Coordinates
        GLfloat nx, ny, nz, nw; // Normal
        GLfloat cx, cy, cz, cw; // Color
        GLfloat tx, ty, tz, tw; // Texture Coordinates
    };


public:
    VertexBufferObjectAttribs();
    ~VertexBufferObjectAttribs();

    void bind();
    void release();
    void render();
    
	GLuint createVBO(GLenum target, GLuint dataSize, const void* data, GLenum usage);
    void setData(const DATA *data, GLenum usage, GLuint nrVertices, GLenum primitiveMode);
    void setIndexData(const GLvoid *data, GLenum usage, GLint nrIndices);
    void addAttrib(GLint attribLoc);
    void setVerticesToRender(GLuint nrVertices); 
    void bindAttribs();
    void setDynamicRendering(GLboolean dynamicRendering);

    GLuint nrVertices() const;
    GLuint nrDynamicVertices() const;


private:
    GLuint m_nrVertices;
    GLuint m_nrDynamicVertices;
    GLuint m_nrIndices;
    GLuint m_nrDynamicIndices;
    GLuint m_sizeBytesVertices;
    GLuint m_sizeBytesIndices;
    GLboolean m_useIndexBuffer;
    
    GLuint m_bufferId;
    GLuint m_indexBufferId;
    
    GLenum m_primitiveMode;
    GLenum m_usage;

    GLuint m_sizeAsStride;

	std::vector<GLint> m_attribLocations;

    GLboolean m_dynamicRendering;   
};

#endif

