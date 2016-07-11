//Author: Sören Pirk
//Date: 22.01.2013

#ifndef GRAPHICALUSERINTERFACE
#define GRAPHICALUSERINTERFACE

#include <glm/vec4.hpp>
#include <string>
#include <vector>

class CameraManager;
class VertexBufferObjectAttribs;
class Shader;
class GUIElement;
class Scene;

class GUI
{
public:
   GUI();
   ~GUI();

   void render();
   void toggleMode();
   int currentMode();
   void setFontColor(const glm::vec4 &color);
   void resize(int width, int height);

   bool onMouseClick(int mx, int my);
   void onMouseMove(int mx, int my);
   void onMouseRelease(); 

private:
	void initShortcuts();
	void initUIElements();
	void initShaders();

private:

	int m_width;
	int m_height;

	std::vector<std::string> m_shortcuts;

	//QTimer m_timer;

	//uint m_fpsCount;
	//uint m_fps;

	int m_mode;

 //   HPTimer m_hpTimer;
    double m_oldTime;


	glm::vec4 m_fontColor;

	VertexBufferObjectAttribs *m_vboQuad;

	Shader *m_shaderSlider;
	Shader *m_shaderCheckBox;
	Shader *m_shaderButton;


	std::vector<GUIElement*> m_guiElements;
};

#endif

