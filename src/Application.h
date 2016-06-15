// Computer Graphics and Media Informatics
// University of Konstanz
// 2015

#ifndef APPLICATION
#define APPLICATION

#include "Common.h"
#include "Global.h"

class VertexBufferObjectAttribs;
class Shader;

/*
-----------------------------------------------------------------------------
							Application.h
-----------------------------------------------------------------------------

This class is responsible for the rendering.

For a more detailed description of the methods,
please see the Application.cpp file!

-----------------------------------------------------------------------------
*/

class Application
{

public:

	Application(int width, int height);
	~Application();

	// Methods

    void init();
    void render();
	void reloadShader();
	void onKey(GLint key, GLint scancode, GLint action, GLint mods);

	// Shader class - stores vertex and fragment programs.
	Shader *m_shader;

	// VertexBufferObject class - stores geometry data.
	VertexBufferObjectAttribs *m_vbo;

	// Switch that toggles between wireframe and solid rendering.
	bool m_showMesh;

	// Width of the frame buffer rendering target.
    int m_framebufferWidth;

	// Height of the frame buffer rendering target.
    int m_framebufferHeight;
    
	// Width of the rendering application window.
	int m_width;

	// Height of the rendering application window.
	int m_height;

	// Zoom factor - alternates distance from camera to the origin.
	float m_zoom;

	// Rotation vector - defined rotation around x and y axis of the geometry.
	glm::vec2 m_rotate;
};

#endif