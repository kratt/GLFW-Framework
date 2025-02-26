
#ifndef CAMERAMANAGER
#define CAMERAMANAGER

//#include "Misc.h"
//#include "Camera.h"

#include "Transform.h"

#include <glm/vec3.hpp> 
#include <vector>

class Camera;

class CameraManager
{	
public:
   CameraManager();
   ~CameraManager();

   void currentPerspective(Transform &trans);
   void resize(float width, float height);
   void onMouseMove(float dx, float dy, int button);
   void onKeyPress(int keyId);
   void onKeyRelease(int keyId);
   void update();
   void onMouseWheel(int dir);
   void toggleCam();
   void renderCameras();
   void increaseSpeed();
   void decreaseSpeed();
   glm::vec3 lodCamPosition();
   void toggleInterpolation();
   void addFrame();
   void clearFrameset();
   void saveFrameset();
   void toggleFrameset();
   std::string currentFramesetName();
   Camera *lodCamera();
   std::vector<Camera*> cameras();
   Camera *currentCam();
   glm::vec3 currentCamPos();  
   float currentCamFov();
   float currentCamNcp();
   float currentCamFcp();

   void currentCamParams();

private:
	
	std::vector<Camera *> m_cameras;

	int m_active;
	float m_width;
	float m_height;
	float m_aspect;
	float m_mouseSensivity;
	float m_camSpeed;
	bool  m_useCam;

    glm::vec3  m_rotate;
    glm::vec3  m_translate;
	float m_zoom;
    float m_fov;
    float m_ncp;
    float m_fcp;
};

#endif

