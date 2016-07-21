// Computer Graphics and Media Informatics
// University of Konstanz
// 2015

#include <stdio.h>
#include "VertexBufferObjectAttribs.h"
#include <iostream>

VertexBufferObjectAttribs::VertexBufferObjectAttribs()
: m_useIndexBuffer(GL_FALSE),
  m_nrVertices(0),
  m_nrDynamicVertices(0),
  m_nrIndices(0),
  m_nrDynamicIndices(0),
  m_sizeBytesVertices(0),
  m_sizeBytesIndices(0),   
  m_bufferId(GL_INVALID_VALUE),
  m_indexBufferId(GL_INVALID_VALUE),
  m_primitiveMode(GL_POINTS),
  m_usage(GL_STATIC_DRAW),
  m_sizeAsStride(0),
  m_dynamicRendering(GL_FALSE)
{  
}

GLuint VertexBufferObjectAttribs::createVBO(GLenum target, GLuint dataSize, const void* data, GLenum usage)
{
    GLuint vbo;

	glGenBuffers(1, &vbo);
	glBindBuffer(target, vbo);
	glBufferData(target, dataSize, data, usage);
	glBindBuffer(target, 0);

    return vbo;
}

VertexBufferObjectAttribs::~VertexBufferObjectAttribs()
{
	if (m_bufferId != GL_INVALID_VALUE) {
		glDeleteBuffers(1, &m_bufferId) ;
	}

	if (m_indexBufferId != GL_INVALID_VALUE) {
		glDeleteBuffers(1, &m_indexBufferId);
	}
}

void VertexBufferObjectAttribs::bindAttribs()
{
    GLint size = m_attribLocations.size();
	glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
  
    for(GLint i=0; i<size; ++i)
    {
        GLint attribLoc = m_attribLocations[i];
		glVertexAttribPointer(attribLoc, 4, GL_FLOAT, GL_FALSE, sizeof(DATA), ((GLchar*)NULL + 4 * sizeof(GLfloat) * i));
    }
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBufferObjectAttribs::render()
{    
    GLint size = m_attribLocations.size();
	glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
  
    for(GLint i=0; i<size; ++i)
    {
        GLint attribLoc = m_attribLocations[i];
		glVertexAttribPointer(attribLoc, 4, GL_FLOAT, GL_FALSE, sizeof(DATA), ((GLchar*)NULL + 4 * sizeof(GLfloat) * i));
    }
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	for (GLint i = 0; i < size; ++i) {
		glEnableVertexAttribArray(m_attribLocations[i]);
	}


    if(m_useIndexBuffer)
    {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
        
        if(m_dynamicRendering)
        {
			glDrawElements(m_primitiveMode, m_nrDynamicIndices, GL_UNSIGNED_INT, 0);
        }
        else
        {
			glDrawElements(m_primitiveMode, m_nrIndices, GL_UNSIGNED_INT, 0);
        }
    }
    else
    {
        if(m_dynamicRendering)
        {
			glDrawArrays(m_primitiveMode, 0, m_nrDynamicVertices);
        }
        else
        {
			glDrawArrays(m_primitiveMode, 0, m_nrVertices);
        }
    }

	
	for (GLint i = 0; i < size; ++i) {
		glDisableVertexAttribArray(m_attribLocations[i]);
	}

}

void VertexBufferObjectAttribs::setData(const DATA *data, GLenum usage, GLuint nrVertices, GLenum primitiveMode)
{
    m_sizeAsStride = sizeof(DATA);
    m_sizeBytesVertices = m_sizeAsStride * nrVertices;
    m_primitiveMode = primitiveMode;
    m_usage = usage;
    m_nrVertices = nrVertices;
    m_nrDynamicVertices = nrVertices;

    m_bufferId = createVBO(GL_ARRAY_BUFFER, m_sizeBytesVertices, data, usage);
}

void VertexBufferObjectAttribs::addAttrib(GLint attribLoc)
{
    m_attribLocations.push_back(attribLoc);
}

void VertexBufferObjectAttribs::setIndexData(const GLvoid *data, GLenum usage, GLint nrIndices)
{
    m_nrIndices = nrIndices;
    m_useIndexBuffer = GL_TRUE;

    m_sizeBytesIndices = sizeof(GLuint) * nrIndices;
    
    m_indexBufferId = createVBO(GL_ELEMENT_ARRAY_BUFFER, m_sizeBytesIndices, data, usage); 
}

void VertexBufferObjectAttribs::setVerticesToRender(GLuint nrVertices)
{
    m_nrDynamicVertices = nrVertices;
}

void VertexBufferObjectAttribs::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
}

void VertexBufferObjectAttribs::release()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBufferObjectAttribs::setDynamicRendering(GLboolean dynamicRendering)
{
    m_dynamicRendering = dynamicRendering;
}

GLuint VertexBufferObjectAttribs::nrVertices() const
{
    return m_nrVertices;
}

GLuint VertexBufferObjectAttribs::nrDynamicVertices() const
{
    return m_nrDynamicVertices;
}