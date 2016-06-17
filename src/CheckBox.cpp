//Author: Sören Pirk
//Date: 22.01.2013

#include "CheckBox.h"

CheckBox::CheckBox(int px, int py, int w, int h, std::string text)
: m_width(w), 
  m_height(h),
  m_posX(px),
  m_posY(py),
  m_mouseClick(false),
  m_text(text),
  m_variable(NULL),
  m_state(false),
  m_color(1.0f, 1.0f, 1.0f, 1.0f)
{    
}

CheckBox::~CheckBox()
{
}

void CheckBox::render()
{
    //glEnable2D();

    //float x1 = m_posX;
    //float x2 = m_posX + m_width;
    //float y1 = m_posY;
    //float y2 = m_posY + m_height;
    //
    //glColor4f(m_color.x, m_color.y, m_color.z,  m_color.w);
    //
    //glBegin(GL_LINES);
    //    glVertex2f(x1, y1);
    //    glVertex2f(x2, y1);

    //    glVertex2f(x1-1, y2);
    //    glVertex2f(x2, y2);
    //    
    //    glVertex2f(x1, y1);
    //    glVertex2f(x1, y2);

    //    glVertex2f(x2, y1);
    //    glVertex2f(x2, y2);
    //glEnd(); 

    //if(m_state)
    //{
    //    glBegin(GL_QUADS);
    //        glVertex2f(x1+1, y1+2);
    //        glVertex2f(x1+1, y2-1);
    //        glVertex2f(x2-2, y2-1);            
    //        glVertex2f(x2-2, y1+2);
    //    glEnd();
    //}

    //glDisable2D();

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