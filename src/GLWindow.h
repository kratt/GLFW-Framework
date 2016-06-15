// Computer Graphics and Media Informatics
// University of Konstanz
// 2015

#ifndef APPLICATION
#define APPLICATION

#include "Common.h"
#include "Global.h"

class VertexBufferObjectAttribs;
class Shader;
class Camera;
class Scene;
class Renderer;
class GUI;
class CameraManager;

#include "Global.h"


class GLWindow
{

public:

	GLWindow(int width, int height);
	~GLWindow();

    void init();
	void initGLFW();

	void initParams();
    void render();


	void keyPressEvent();
	void keyReleaseEvent();
	void mouseMoveEvent();
	void mousePressEvent();
	void mouseReleaseEvent();
	void mouseWheelEvent();

	void resize(int width, int height);

private:
	int m_width;
	int m_height;


private:
	Scene *m_scene;
	Renderer *m_renderer;
	CameraManager *m_cameraManager;

	GUI *m_gui;
	
};

#endif