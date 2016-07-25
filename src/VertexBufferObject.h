// 25.07.2016
// author: Julian Kratt

#ifndef VERTEX_BUFFER_OBJECT_H
#define VERTEX_BUFFER_OBJECT_H

#include "Common.h"
#include "Global.h"
#include "VertexData.h"

#include <vector>

class VertexBufferObject
{

public:
	VertexBufferObject();
    ~VertexBufferObject();

    void bind();
    void unbind();
    void render();
    
	GLuint createVBO(GLenum target, GLuint dataSize, const  std::vector<VertexData> &data, GLenum usage);
    void setData(const std::vector<VertexData> &data, GLenum usage, GLuint nrVertices, GLenum primitiveMode);
    void setIndexData(const std::vector<VertexData> &data, GLenum usage, GLint nrIndices);
    void addAttrib(GLint attribLoc);
    void setVerticesToRender(GLuint nrVertices); 
    void bindAttribs();
    void setDynamicRendering(GLboolean dynamicRendering);

    GLuint nrVertices() const;
    GLuint nrDynamicVertices() const;

private:
	void initVAO();

private:
	GLuint m_vaoId;

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

