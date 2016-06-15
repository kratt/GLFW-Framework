//Author: Sören Pirk
//Date: 22.01.2013

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

    glm::vec4 m_bgColor; 

    const GLuint m_samples;
    GLuint m_bgMode;
};

#endif

 