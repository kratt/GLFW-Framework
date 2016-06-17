//Author: Sören Pirk
//Date: 22.01.2013

#ifndef GUIELEMENT
#define GUIELEMENT


class GUIElement
{
public:
    GUIElement(){};
    virtual ~GUIElement(){};

   virtual void render() = 0;
   virtual bool onMouseClick(int mx, int my) = 0;
   virtual void onMouseMove(int mx, int my) = 0;
   virtual void onMouseRelease() = 0;

private:  
    
};

#endif

