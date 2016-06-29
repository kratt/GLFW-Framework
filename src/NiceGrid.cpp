#include "NiceGrid.h"
#include "Shader.h"
#include "VertexBufferObjectAttribs.h"
#include "Texture.h"
#include "Mesh.h"
#include "RenderContext.h"
#include "PngLoader.h"
#include "Texture.h"

NiceGrid::NiceGrid(GLfloat size, GLfloat rep)
: m_size(size),
  m_rep(rep),
  m_shader(NULL),
	m_texID(0),
  m_backFaceAlpha(0.2),
  m_ambient(0.12f, 0.12f, 0.1f),
  m_diffuse(1.0f, 1.0f, 0.9f),
  m_vbo(NULL),
  m_position(0.0f, 0.0f, 0.0f)
{
	//m_tex = new Texture("../Data/Textures/floor_test.png");

	//m_texID = PngLoader::loadTexture("../Data/Textures/floor_blue.png");
    //QImage tex("Data/floor_blue.png");

    //m_texture = new Texture(tex);
    //m_texture->setWrapMode(GL_REPEAT);

    m_shader = new Shader("../Shader/NiceGrid.vert.glsl", "../Shader/NiceGrid.frag.glsl");
    m_shader->bindAttribLocations();

    createVBO();
}

NiceGrid::~NiceGrid()
{
    delete m_shader;
}

void NiceGrid::render()
{
	auto param = RenderContext::globalObjectParam();
	auto trans = RenderContext::transform();

    glm::mat4 model = glm::translate(glm::mat4(1.0f), m_position);
    glm::mat4 view = trans->view;
	glm::mat4 projection = trans->projection;
	glm::mat4 viewProjection = trans->viewProjection;
	glm::mat4 lightView = trans->lightViewProjectionBias;

    m_shader->bind();  

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_tex->id());    
        m_shader->seti("tex", 0);    

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, param->shadowMapID);    
        m_shader->seti("shadowMap", 1);   

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, param->shadowMapBlurredID);    
        m_shader->seti("shadowMapBlurred", 2);   

        m_shader->set3f("lightPos", param->lightPos);
        m_shader->set3f("lightDir", param->lightDir);
        m_shader->set3f("camPos", param->camPos);
        m_shader->set3f("diffuseColor", m_diffuse.x, m_diffuse.y, m_diffuse.z);
        m_shader->set3f("ambientColor", m_ambient.x, m_ambient.z, m_ambient.z);
        m_shader->setf("alpha", 1.0);
        m_shader->seti("applyShadow", param->applyShadow);
        m_shader->seti("renderMode", param->gridRenderMode);
        m_shader->setf("shadowIntensity", param->shadowIntensity);

		m_shader->setMatrix("matModel", model, GL_FALSE);
		m_shader->setMatrix("matView", view, GL_FALSE);
		m_shader->setMatrix("matProjection", projection, GL_FALSE);
		m_shader->setMatrix("matLightView", lightView, GL_FALSE);
		m_shader->setMatrix("matViewProjection", viewProjection, GL_FALSE);

        m_vbo->render();

    m_shader->release();
}

void NiceGrid::setAmbientColor(float r, float g, float b)
{
    m_ambient.x = r;
    m_ambient.y = g;
    m_ambient.z = b;
}

void NiceGrid::setDiffuseColor(float r, float g, float b)
{
    m_diffuse.x = r;
    m_diffuse.y = g;
    m_diffuse.z = b;
}

void NiceGrid::createVBO()
{
    int nrVertices = 4;
    VertexBufferObjectAttribs::DATA *data = new VertexBufferObjectAttribs::DATA[nrVertices];

	// v0
    data[0].vx = -m_size;
    data[0].vy = 0.0f;
    data[0].vz = m_size;
    data[0].vw = 1.0f;

    data[0].nx = 0.0f;
    data[0].ny = 1.0f;
    data[0].nz = 0.0f;
    data[0].nw = 0.0f;

    data[0].cx = 1.0f;
    data[0].cy = 1.0f;
    data[0].cz = 1.0f;
    data[0].cw = 1.0f;

    data[0].tx = 0.0f;
    data[0].ty = 0.0f;
    data[0].tz = 0.0f;
    data[0].tw = 0.0f;

	//v1
	data[1].vx = m_size;
	data[1].vy = 0.0f;
	data[1].vz = m_size;
	data[1].vw = 1.0f;
		 
	data[1].nx = 0.0f;
	data[1].ny = 1.0f;
	data[1].nz = 0.0f;
	data[1].nw = 0.0f;
		 
	data[1].cx = 1.0f;
	data[1].cy = 1.0f;
	data[1].cz = 1.0f;
	data[1].cw = 1.0f;
		 
	data[1].tx = m_rep;
	data[1].ty = 0.0f;
	data[1].tz = 0.0f;
	data[1].tw = 0.0f;

	//v2
    data[2].vx = -m_size;
    data[2].vy = 0.0f;
    data[2].vz = -m_size;
    data[2].vw = 1.0f;
		 
    data[2].nx = 0.0f;
    data[2].ny = 1.0f;
    data[2].nz = 0.0f;
    data[2].nw = 0.0f;
		 
    data[2].cx = 1.0f;
    data[2].cy = 1.0f;
    data[2].cz = 1.0f;
    data[2].cw = 1.0f;
		 
    data[2].tx = 0.0f;
    data[2].ty = m_rep;
    data[2].tz = 0.0f;
    data[2].tw = 0.0f;

	//v3
    data[3].vx = m_size;
    data[3].vy = 0.0f;
    data[3].vz = -m_size;
    data[3].vw = 1.0f;
		 
    data[3].nx = 0.0f;
    data[3].ny = 1.0f;
    data[3].nz = 0.0f;
    data[3].nw = 0.0f;
		 
    data[3].cx = 1.0f;
    data[3].cy = 1.0f;
    data[3].cz = 1.0f;
    data[3].cw = 1.0f;
		 
    data[3].tx = m_rep;
    data[3].ty = m_rep;
    data[3].tz = 0.0f;
    data[3].tw = 0.0f;

    m_vbo = new VertexBufferObjectAttribs();
    m_vbo->setData(data, GL_STATIC_DRAW, nrVertices, GL_TRIANGLE_STRIP);

    m_vbo->addAttrib(VERTEX_POSITION);
    m_vbo->addAttrib(VERTEX_NORMAL);
    m_vbo->addAttrib(VERTEX_COLOR);
    m_vbo->addAttrib(VERTEX_TEXTURE);
    m_vbo->bindAttribs();

    delete[] data;
}