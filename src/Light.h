//Author: Sören Pirk
//Date: 22.01.2013

#ifndef LIGHT
#define LIGHT

#include "Spline.h"
#include <string>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class CameraManager;
class VertexBufferObjectAttribs;
class Shader;
class FrameBufferObject;
class Scene;

class Light
{
public:
	struct Path
	{
		std::string name;
		std::vector<glm::vec3> positions;
	};

   Light(Scene *scene, const glm::vec3 &pos);
   ~Light();
    
   void blurShadowMap();
   void renderLightView();
   void setLightView();

   void render();
   glm::vec3 position();

   void setPosition(const glm::vec3 &pos);
   void move(CameraManager *camManager, float diffX, float diffY);

   void loadPaths();
   void savePaths();
   void recordPath(bool record);
   void autoMove();
   void toggleMode();
   bool hasMoved();

   void update(float delta);

   bool m_moved;

   FrameBufferObject *m_fboLight;
   FrameBufferObject *m_fboBlurV;
   FrameBufferObject *m_fboBlurH;

private:

	glm::vec3 m_position;
	float m_renderSize;

	std::vector<Path> m_paths;
    Path m_curPath;

    bool m_record;
    bool m_first;
    bool m_saved;

    //DWORD m_oldTime;

    int m_moveMode;

    float m_height;
    float m_angle;
    float m_radius;

    glm::vec3 m_oldPosition;
    //Spline m_spline;
    float m_time;

    float m_speed;
    float m_distance;
    float m_movement;

    VertexBufferObjectAttribs *m_vbo;
    VertexBufferObjectAttribs *m_vboBlur;

    Shader *m_shader;    
    Shader *m_shaderBlur;    


public:
 

    int m_bufferWidth;
    int m_bufferHeight;
    float m_fcpLight;
    float m_ncpLight;
    float m_fovLight;

    Scene *m_scene;

    glm::mat4x4 m_lightView;
};

#endif

