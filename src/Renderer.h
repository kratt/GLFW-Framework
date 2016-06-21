#ifndef RENDERER
#define RENDERER

#include "Global.h"
#include <glm/vec4.hpp> 

class GUI;
class CameraManager;
class FrameBufferObjectMultisample;
class FrameBufferObject;
class Shader;
class Scene;
class VertexBufferObjectAttribs;
class Texture;

class Renderer
{
public:
    Renderer(Scene *scene, CameraManager *camManager);
    ~Renderer();

    void init();
    void render();
	void renderQuad();
    
    void resize(int width, int height);
    void toggleBGColor();
    void togglePolygonMode();

private:
    void renderScene();

private:
    GUI *m_gui;
    CameraManager *m_cameraManager;
	Scene *m_scene;

    int m_width;
    int m_height;

	VertexBufferObjectAttribs* m_vboQuad;
	Shader *m_shaderQuad;

    glm::vec4 m_bgColor; 

    const GLuint m_samples;
    GLuint m_bgMode;

	Texture *m_texTest;
};

#endif

 