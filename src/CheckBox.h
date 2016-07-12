//Author: Sören Pirk
//Date: 22.01.2013

#ifndef CHECKBOX
#define CHECKBOX

#include "Global.h"
#include "GUIElement.h"

#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include <string>

class Shader;
class VertexBufferObjectAttribs;

class CheckBox : public GUIElement
{
public:
   CheckBox(int px, int py, int w, int h, std::string text);
   ~CheckBox();
   
   void render(Shader* shader);
   bool onMouseClick(int mx, int my);
   void onMouseMove(int mx, int my);
   void onMouseRelease();        
   void setPosition(int px, int py);

   void setState(bool value);   
   void setColor(const glm::vec4 &color);
   void setVariable(bool *variable);

   bool state() const;

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

    glm::vec4 m_color;

    bool m_mouseClick;

    bool m_state;
    bool *m_variable;

	VertexBufferObjectAttribs* m_vboLines;
	VertexBufferObjectAttribs* m_vboQuad;
};

#endif

