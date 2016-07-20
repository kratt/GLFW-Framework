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
	void initParams();
    void render();


	void keyPressEvent(GLint key, GLint scancode, GLint action, GLint mods);
	void keyReleaseEvent(GLint key, GLint scancode, GLint action, GLint mods);
	void mouseMoveEvent(GLdouble x, GLdouble y);
	void mousePressEvent(GLint button, GLint action, GLint mods);
	void mouseWheelEvent(GLdouble xo, GLdouble yo);

	void resize(int width, int height);

private:
	int m_width;
	int m_height;

	glm::vec2 m_mouse;

	bool m_leftButton;
	bool m_rightButton;
	bool m_ctrlPressed;
	bool m_altPressed;
	bool m_shiftPressed;
	bool m_noOtherKey;

private:
	Scene *m_scene;
	Renderer *m_renderer;
	CameraManager *m_cameraManager;

	GUI *m_gui;
};

#endif