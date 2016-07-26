#ifndef NICEGRID
#define NICEGRID

#include "Global.h"
#include <glm/vec3.hpp>

class Shader;
class Texture;
class VertexBufferObject;

class NiceGrid
{
public:
   NiceGrid(GLfloat size, GLfloat rep);
   ~NiceGrid();

   void render();
   void setDiffuseColor(float r, float g, float b);
   void setAmbientColor(float r, float g, float b);  

private:
    void createVBO();


private:
    GLuint  m_texID;
    GLfloat m_size;
    GLfloat m_rep;

	Texture *m_tex;
    Shader *m_shader;
    VertexBufferObject *m_vbo;

    glm::vec3 m_ambient;
	glm::vec3 m_diffuse;

	glm::vec3 m_position;

    float m_backFaceAlpha;
};

#endif

