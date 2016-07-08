#ifndef CAMERA
#define CAMERA


#include "Transform.h"
#include "HPTimer.h"

#include <math.h>
#include <glm/vec3.hpp> 
#include <glm/glm.hpp>

#include <vector>

class Frustum;
class Spline;

class Camera
{
private:
	float m_heading;
	float m_pitch;
	float m_roll;
	float m_forwardSpeed;
	float m_strafeSpeed;

	glm::vec3 m_pos;
	glm::vec3 m_dir;
	glm::vec3 m_realPos;
	glm::vec3 m_strafe;
	glm::vec3 m_up;
	glm::vec3 m_tempDir;

	glm::vec3 m_cameraColor;

	glm::vec3 m_fixUp;
	glm::vec3 m_fixRight;
	glm::vec3 m_fixDir;

    float m_matrix1[16];
    float m_matrix2[16];

	glm::mat4 m_rotMat;

    float m_lerpFactor;
    float m_ncp;
    float m_fcp;
    float m_fov;
	float m_aspect;

    float m_moveSpeed;
    float m_time;

    Frustum *m_frustum;
	Frustum *m_showFrustum;

    bool m_renderFrustum;
    bool m_interpolate;

    struct CameraFrame
    {
		glm::vec3 pos;
        float  headingDeg;
        float  pitchDeg;
		float  desiredDistance;
    };
    
    HPTimer m_hpTimer;

    float m_timer;
    float m_timerDiff; 
    float m_secsPerFrame;
    float m_desiredDistance;
    float m_movementValue;

    std::vector<float> m_secsPerFrameList;

    Spline *m_spline;
    Spline *m_splineView;
	Spline *m_splineSpeed;

    int m_idxInterpolNew;
    int m_idxInterpolOld;

    std::vector<CameraFrame> m_camFrames;

	int m_activeFrameSet;
	std::string m_activeFrameSetName;
	std::string m_frameSetFolder;
	std::vector<std::string> m_frameFileNames;

public:
	Camera(glm::vec3 pos, float heading, float pitch, float roll, float fov, float ncp, float fcp);
	virtual ~Camera();

    void moveForward(bool move);
    void moveBackward(bool move);
    void strafeLeft(bool move);
    void strafeRight(bool move);
    void increaseDistPerSec(float delta);
    void decreaseDistPerSec(float delta);
    void setDistPerSec(float value);

	void changeHeading(float degrees);
	void changePitch(float degrees);
    void changeRoll(float degrees);

	void setPerspective(Transform &trans);
    void render();
    void update();
	glm::vec3 position() const;
	glm::vec3 direction() const;
	glm::vec3 up() const;
	glm::vec3 right() const;
    float pitch() const;
    float heading() const;
    float roll() const;
    float fov() const;
    float ncp() const;
    float fcp() const;

    void interpolate(float speed);
    void splineInterpolation();
    void updateCamInternals(float ratio);
    void renderFrames();
    void renderSpline();
    void setPosition(float x, float y, float z);
    void setColor(float r, float g, float b);
	void setHeading(float degrees);
	void setPitch(float degrees);
	void setSpeed(float s);
    
	void addFrame(glm::vec3 pos, float heading, float pitch, float desiredDistance);
    void autoAddFrame();    
    void clearFrames();
    void saveFrames(const std::string &filePath);
	void saveFrames();
    void loadFrames(const std::string &filePath);
	void loadFrameDirectory(const std::string &folderPath);
	void changeFrameSet();
    void toggleInterpolation();

    Frustum *frustum();

    void determineMovement();
    void interpolate();
	std::string frameSetName() const;
};

#endif