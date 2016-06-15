#ifndef SCENE
#define SCENE


class NiceGrid;
class Light;
class VertexBufferObjectAttribs;
class Shader;
class CameraManager;


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

    Shader *m_shaderNormal;
    Shader *m_shaderDepth;

    int m_activeIdx;
};

#endif

 