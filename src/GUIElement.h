#ifndef GUIELEMENT
#define GUIELEMENT

#include "gui_type.h"

class Shader;

class GUIElement
{

public:
    GUIElement(GUI_TYPE type): m_type(type){};
    virtual ~GUIElement(){};

   virtual void render(Shader* shader) = 0;
   virtual bool onMouseClick(int mx, int my) = 0;
   virtual void onMouseMove(int mx, int my) = 0;
   virtual void onMouseRelease() = 0;

   GUI_TYPE type() {
	   return m_type;
   };

private:  
	GUI_TYPE m_type;
    
};

#endif

