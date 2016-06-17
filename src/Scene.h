#ifndef SCENE
#define SCENE


class NiceGrid;
class Light;
class VertexBufferObjectAttribs;
class Shader;
class CameraManager;
class Object;

#include <vector>

class Scene
{
public:
    Scene(CameraManager *camManager);
    ~Scene();

	void update(float delta);
    void init();
    void initShaders();

    void renderWorld();  
    void renderObjects();  
    void renderObjectsDepth();

    void select(int sw, int sh, int mx, int my);
    void move(int x, int y);
    void resetSelection();

public:
    Light *m_light;

private:
	
    NiceGrid *m_niceGrid;
    CameraManager *m_cameraManager;
    VertexBufferObjectAttribs *m_vbo;
	VertexBufferObjectAttribs *m_vboQuad;

	std::vector<Object*> m_objects;

    Shader *m_shaderNormal;
    Shader *m_shaderDepth;
	Shader *m_shaderQuad;

    int m_activeIdx;
};

#endif

 