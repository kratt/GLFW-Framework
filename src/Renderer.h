#ifndef RENDERER
#define RENDERER

#include "Global.h"
#include <glm/vec4.hpp> 


class CameraManager;
class FrameBufferObjectMultisample;
class FrameBufferObject;
class Shader;
class Scene;
class VertexBufferObjectAttribs;

class Renderer
{
public:
    Renderer(Scene *scene, CameraManager *camManager);
    ~Renderer();

    void render();
    
    void resize(int width, int height);
    void toggleBGColor();

private:
    void renderScene();

private:
    CameraManager *m_cameraManager;
	Scene *m_scene;

    int m_width;
    int m_height;

    glm::vec4 m_bgColor; 

    GLuint m_bgMode;
};

#endif

 