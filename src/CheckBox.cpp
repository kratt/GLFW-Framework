#include "CheckBox.h"
#include "Shader.h"
#include "VertexBufferObject.h"
#include "VertexData.h"
#include "Mesh.h"
#include "RenderContext.h"
#include "gui_type.h"

CheckBox::CheckBox(int px, int py, int w, int h, std::string text)
: m_width(w), 
  m_height(h),
  m_posX(px),
  m_posY(py),
  m_mouseClick(false),
  m_text(text),
  m_variable(nullptr),
  m_state(false),
  m_color(1.0f, 1.0f, 1.0f, 1.0f),
  m_vboLines(nullptr),
  m_vboQuad(nullptr),
  GUIElement(GUI_TYPE::GUI_CHECKBOX)
{    
	initVBOs();
}

CheckBox::~CheckBox()
{
}

void CheckBox::render(Shader* shader)
{
	auto param = RenderContext::globalObjectParam();

	shader->bind();

	shader->seti("windowWidth", param->windowWidth);
	shader->seti("windowHeight", param->windowHeight);
	shader->seti("renderLines", true);

	m_vboLines->render();
	if (m_state){
		m_vboQuad->render();
	}
	shader->release();

    //QString sliderText = m_text;
    //renderString(sliderText.toAscii(), m_posX+m_width+3, m_posY+12, m_color.x, m_color.y, m_color.z, m_color.w, GLUT_BITMAP_HELVETICA_12);
}

void CheckBox::onMouseMove(int mx, int my)
{
}

bool CheckBox::onMouseClick(int mx, int my)
{
    float x1 = m_posX;
    float x2 = m_posX + m_width;
    float y1 = m_posY;
    float y2 = m_posY + m_height;

    m_mouseClick = false;

    if(mx > x1 && mx < x2)
    {
        if(my > y1 && my < y2)
        {   
            m_state = !m_state;
            
            if(m_variable)
                (*m_variable) = m_state;

            m_mouseClick = true;
        }
    }

    return m_mouseClick;
}

void CheckBox::onMouseRelease()
{
    m_mouseClick = false;
}

bool CheckBox::state() const
{
    return m_state;
}

void CheckBox::setState(bool value)
{
    m_state = value;
    
    if(m_variable)
        *m_variable = value;
}

void CheckBox::setColor(const glm::vec4 &color)
{
    m_color = color;
}

void CheckBox::setVariable(bool *variable)
{
    m_variable = variable;
}

void CheckBox::setPosition(int x, int y)
{
	m_posX = x;
	m_posY = y;
}

glm::vec2 CheckBox::position()
{
	return glm::vec2(float(m_posX), float(m_posY));
}

glm::vec2 CheckBox::dimensions()
{
	return glm::vec2(float(m_width), float(m_height));
}

void CheckBox::initVBOs()
{
	float x1 = m_posX;
	float x2 = m_posX + m_width;
	float y1 = m_posY;
	float y2 = m_posY + m_height;

	// http://www.monkey-x.com/Community/posts.php?topic=3204
	// OpenGL line renderer doesn't work with pixel boundary position
	x1 += 0.5f;
	x2 += 0.5f;
	y1 += 0.5f;
	y2 += 0.5f;

	std::vector<glm::vec2> vertices;

	vertices.push_back(glm::vec2(x1, y1));
	vertices.push_back(glm::vec2(x2, y1));
	vertices.push_back(glm::vec2(x2, y2));
	vertices.push_back(glm::vec2(x1, y2));

	int nrVertices = vertices.size();
	std::vector<VertexData> v_data =  std::vector<VertexData>(nrVertices);

	for (int i = 0; i<nrVertices; ++i)
	{
		glm::vec2 v = vertices[i];

		v_data[i].vx = v.x;
		v_data[i].vy = v.y;
		v_data[i].vz = 0.0f;
		v_data[i].vw = 1.0f;
		
		v_data[i].cx = 0.0f;
		v_data[i].cy = 0.0f;
		v_data[i].cz = 0.0f;
		v_data[i].cw = 0.0f;
		
		v_data[i].nx = 0.0f;
		v_data[i].ny = 0.0f;
		v_data[i].nz = 0.0f;
		v_data[i].nw = 0.0f;
	
		v_data[i].tx = 0.0f;
		v_data[i].ty = 0.0f;
		v_data[i].tz = 0.0f;
		v_data[i].tw = 0.0f;
	}

	delete m_vboLines;
	m_vboLines = new VertexBufferObject();
	m_vboLines->setData(v_data, GL_STATIC_DRAW, nrVertices, GL_LINE_LOOP);

	m_vboLines->addAttrib(VERTEX_POSITION);
	m_vboLines->addAttrib(VERTEX_NORMAL);
	m_vboLines->addAttrib(VERTEX_COLOR);
	m_vboLines->addAttrib(VERTEX_TEXTURE);
	m_vboLines->bindAttribs();


	// quad to render inside of the ckechbox
	delete m_vboQuad;
	m_vboQuad = Mesh::quad(x1 + 2, y1 + 2, m_width - 3, m_height - 3);
}
