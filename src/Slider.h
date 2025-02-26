#ifndef SLIDER_H
#define SLIDER_H

#include "GUIElement.h"
#include "Mesh.h"
#include "Shader.h"
#include "VertexBufferObject.h"
#include "VertexData.h"
#include "gui_type.h"
#include "RenderContext.h"
#include "TextRenderer.h"

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip> 


template <class T> class Slider : public GUIElement
{
public:
   Slider(int px, int py, int w, int h, std::string text);
   ~Slider();

   void render(Shader *shader);
   bool onMouseClick(int mx, int my);
   void onMouseMove(int mx, int my);
   void onMouseRelease();
   void setPosition(int px, int py);

   void setValue(T value);   
   void setRange(T min, T max);
   void setColor(const glm::vec4 &color);
   void setVariable(T *variable);
   void setPos(int x, int y);

   T value() const;

   glm::vec2 position();
   glm::vec2 dimensions();

private:
	void initVBOs();

private:
    int m_width;
	int m_height;
	int m_posX;
	int m_posY;

    std::string m_text;

    float m_percent;
    float m_range;
    T m_valueRange;
    T m_value;
    T m_minValue;
    T m_maxValue;
	glm::vec4 m_color;

    bool m_mouseClick;

    T *m_variable;   

	VertexBufferObject* m_vboLines;
	VertexBufferObject* m_vboQuad;
};


template <class T> Slider<T>::Slider(int px, int py, int w, int h, std::string text)
	: m_width(w),
	m_height(h),
	m_posX(px),
	m_posY(py),
	m_mouseClick(false),
	m_text(text),
	m_minValue(static_cast<T>(0.0f)),
	m_maxValue(static_cast<T>(1.0f)),
	m_value(static_cast<T>(0.0f)),
	m_valueRange(static_cast<T>(0.0f)),
	m_variable(nullptr),
	m_color(1.0f, 1.0f, 1.0f, 1.0f),
	m_vboLines(nullptr),
	m_vboQuad(nullptr),
	GUIElement(GUI_TYPE::GUI_SLIDER)
{
	m_range = m_width;
	m_percent = 1.0f;

	m_valueRange = m_maxValue - m_minValue;

	initVBOs();
}

template <class T>
Slider<T>::~Slider() {}

template <class T>
void Slider<T>::render(Shader *shader)
{
	auto param = RenderContext::globalObjectParam();

	glm::vec2 scale = glm::vec2((m_width - 3) * m_percent, m_height - 3);
	glm::vec2 trans = glm::vec2(float(m_posX + 2), float(m_posY + 2));

	shader->bind();

	shader->seti("windowWidth", param->windowWidth);
	shader->seti("windowHeight", param->windowHeight);
	shader->set2f("scale", scale);
	shader->set2f("trans", trans);

	shader->seti("renderLines", true);

		m_vboLines->render();

	shader->seti("renderLines", false);
		m_vboQuad->render();

	shader->release();


	std::stringstream ss;
	ss << std::fixed << std::setprecision(3) << m_value;
	std::string mystring = ss.str();


	std::string sliderText = m_text;
	sliderText += ss.str();

	TextRenderer::instance()->render(sliderText, glm::vec2(m_posX + 2, param->windowHeight - m_posY + 5), 16, m_color, "calibril");
}

template <class T>
void Slider<T>::onMouseMove(int mx, int my)
{
	float x1 = m_posX;
	float x2 = m_posX + m_width;

	if (m_mouseClick)
	{
		int curPosX = 0;

		if (mx > x1 && mx < x2)
		{
			curPosX = mx - m_posX;
			m_percent = (float)curPosX / m_range;
		}

		if (mx < x1)
		{
			m_percent = 0.0f;
		}
		if (mx > x2)
		{
			m_percent = 1.0f;
		}

		m_value = static_cast<T>(m_minValue + m_valueRange * m_percent);
		//m_value = static_cast<T>( 1024* int(100 * m_percent) );
		//if(m_value < m_minValue) m_value = m_minValue;

		if (m_variable)
			*m_variable = m_value;
	}
}

template <class T>
bool Slider<T>::onMouseClick(int mx, int my)
{
	float x1 = m_posX;
	float x2 = m_posX + m_width;
	float y1 = m_posY;
	float y2 = m_posY + m_height;

	m_mouseClick = false;

	int curPosX = 0;

	if (mx > x1 && mx < x2)
	{
		if (my > y1 && my < y2)
		{
			m_mouseClick = true;

			curPosX = mx - m_posX;
			m_percent = (float)curPosX / m_range;

			m_value = static_cast<T>(m_minValue + m_valueRange * m_percent);

			if (m_variable)
				*m_variable = m_value;
		}
	}

	return m_mouseClick;
}

template <class T>
void Slider<T>::onMouseRelease()
{
	m_mouseClick = false;
}

template <class T>
void Slider<T>::setRange(T min, T max)
{
	if (min < max && max > static_cast<T>(0.0f))
	{
		m_minValue = min;
		m_maxValue = max;

		m_valueRange = max - min;
		m_value = min + m_valueRange * m_percent;

		if (m_variable)
			*m_variable = m_value;
	}
	else
	{
		float tmpMin = static_cast<float>(min);
		float tmpMax = static_cast<float>(max);

		if (tmpMin < tmpMax)
		{
			m_minValue = min;
			m_maxValue = max;

			m_valueRange = static_cast<T>(tmpMax - tmpMin);
			m_value = static_cast<T>(m_minValue + m_valueRange * m_percent);

			if (m_variable)
				*m_variable = m_value;
		}
		else
		{
			m_minValue = max;
			m_maxValue = min;

			m_valueRange = static_cast<T>(tmpMin - tmpMax);
			m_value = static_cast<T>(m_minValue + m_valueRange * m_percent);

			if (m_variable)
				*m_variable = m_value;
		}
	}
}

template <class T>
T Slider<T>::value() const
{
	return m_value;
}

template <class T>
void Slider<T>::setValue(T value)
{
	m_value = value;

	if (m_value < m_minValue)
		m_value = m_minValue;

	if (m_value > m_maxValue)
		m_value = m_maxValue;

	if (m_variable)
		*m_variable = value;

	if (value < static_cast<T>(0.0f))
	{
		m_percent = (abs(static_cast<float>(m_minValue)) - fabs(static_cast<float>(m_value))) / static_cast<float>(m_valueRange);
	}
	else {
		m_percent = fabs(static_cast<float>(m_value) - static_cast<float>(m_minValue)) / static_cast<float>(m_valueRange);
	}
}

template <class T>
void Slider<T>::setColor(const glm::vec4 &color)
{
	m_color = color;
}

template <class T>
void Slider<T>::setVariable(T *variable)
{
	m_variable = variable;
}

template <class T>
void Slider<T>::setPosition(int x, int y)
{
	m_posX = x;
	m_posY = y;
}

template <class T>
glm::vec2 Slider<T>::position()
{
	return glm::vec2(float(m_posX), float(m_posY));
}

template <class T>
glm::vec2 Slider<T>::dimensions()
{
	return glm::vec2(float(m_width), float(m_height));
}

template<class T>
inline void Slider<T>::initVBOs()
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
	std::vector<VertexData> v_data = std::vector<VertexData>(nrVertices);

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


	// quad to render inside of slider. Scale it according to needs
	delete m_vboQuad;
	m_vboQuad = Mesh::quad(0, 0, 1, 1);
}


#endif

