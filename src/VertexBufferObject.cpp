#include "VertexBufferObject.h"
#include "log/Log.h"

#include <stdio.h>
#include <iostream>

VertexBufferObject::VertexBufferObject()
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
  m_dynamicRendering(GL_FALSE),
  m_vaoId(0)
{  
	glGenVertexArrays(1, &m_vaoId);
}

GLuint VertexBufferObject::createVBO(GLenum target, GLuint dataSize, const std::vector<VertexData> &data, GLenum usage)
{
    GLuint vbo;

	glGenBuffers(1, &vbo);
	glBindBuffer(target, vbo);
	glBufferData(target, dataSize, data.data(), usage);
	glBindBuffer(target, 0);

    return vbo;
}

VertexBufferObject::~VertexBufferObject()
{
	if (m_bufferId != GL_INVALID_VALUE) {
		glDeleteBuffers(1, &m_bufferId) ;
	}

	if (m_indexBufferId != GL_INVALID_VALUE) {
		glDeleteBuffers(1, &m_indexBufferId);
	}
}

void VertexBufferObject::bindAttribs()
{
	glBindVertexArray(m_vaoId);

	glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
	for (GLint i = 0; i<m_attribLocations.size(); ++i)
	{
		GLint attribLoc = m_attribLocations[i];
		glVertexAttribPointer(attribLoc, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), ((GLchar*)NULL + 4 * sizeof(GLfloat) * i));
		glEnableVertexAttribArray(m_attribLocations[i]);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void VertexBufferObject::render()
{    
	glBindVertexArray(m_vaoId);
		glDrawArrays(m_primitiveMode, 0, m_nrVertices);
	glBindVertexArray(0);


   /* GLint size = m_attribLocations.size();
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
	}*/

}

void VertexBufferObject::setData(const std::vector<VertexData> &data, GLenum usage, GLuint nrVertices, GLenum primitiveMode)
{
    m_sizeAsStride = sizeof(VertexData);
    m_sizeBytesVertices = m_sizeAsStride * nrVertices;
    m_primitiveMode = primitiveMode;
    m_usage = usage;
    m_nrVertices = nrVertices;
    m_nrDynamicVertices = nrVertices;

    m_bufferId = createVBO(GL_ARRAY_BUFFER, m_sizeBytesVertices, data, usage);
}

void VertexBufferObject::addAttrib(GLint attribLoc)
{
    m_attribLocations.push_back(attribLoc);
}

void VertexBufferObject::setIndexData(const std::vector<VertexData> &data, GLenum usage, GLint nrIndices)
{
    m_nrIndices = nrIndices;
    m_useIndexBuffer = GL_TRUE;

    m_sizeBytesIndices = sizeof(GLuint) * nrIndices;
    
    m_indexBufferId = createVBO(GL_ELEMENT_ARRAY_BUFFER, m_sizeBytesIndices, data, usage); 
}

void VertexBufferObject::setVerticesToRender(GLuint nrVertices)
{
    m_nrDynamicVertices = nrVertices;
}

void VertexBufferObject::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
}

void VertexBufferObject::unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBufferObject::setDynamicRendering(GLboolean dynamicRendering)
{
    m_dynamicRendering = dynamicRendering;
}

GLuint VertexBufferObject::nrVertices() const
{
    return m_nrVertices;
}

GLuint VertexBufferObject::nrDynamicVertices() const
{
    return m_nrDynamicVertices;
}