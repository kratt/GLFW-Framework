#include "Camera.h"
#include "Frustum.h"
#include "Spline.h"
#include "RenderContext.h"
#include "Common.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <experimental/filesystem>

Camera::Camera(glm::vec3 pos, float heading, float pitch, float roll, float fov, float ncp, float fcp)
: m_idxInterpolNew(1),
  m_idxInterpolOld(0),
  m_lerpFactor(0.0f),
  m_forwardSpeed(0.0f),
  m_strafeSpeed(0.0f),
  m_heading(heading),
  m_pitch(pitch),
  m_roll(roll),
  m_dir(0.0f, 0.0f, 0.0f),
  m_strafe(0.0f, 0.0f, 0.0f),
  m_up(0.0, 1.0, 0.0),
  m_tempDir(0.0, 0.0, 0.0),
  m_fov(fov), 
  m_ncp(ncp),
  m_fcp(fcp),
  m_frustum(NULL),
  m_renderFrustum(true),
  m_moveSpeed(0.01f),
  m_timer(0.0f),
  m_timerDiff(0.0f),
  m_secsPerFrame(0.0f),
  m_desiredDistance(4.0f),
  m_movementValue(0.0f),
  m_time(0.0f),
  m_cameraColor(1.0f, 1.0f, 1.0f),
  m_interpolate(false),
  m_activeFrameSet(0),
  m_spline(NULL),
  m_splineView(NULL),
  m_splineSpeed(NULL),
  m_activeFrameSetName("No Set Available"),
  m_aspect(1.3f)
{
    m_frustum = new Frustum();
	m_showFrustum = new Frustum();	

    m_frustum->setCamInternals(m_fov, m_aspect, 0.2, m_fcp);
	m_showFrustum->setCamInternals(m_fov, m_aspect, 0.2, m_fcp*0.005f);

	m_spline      = new Spline();
	m_splineView  = new Spline();
	m_splineSpeed = new Spline();

    //!!!!!!!!!!!
    m_pos.x = pos.x;
    m_pos.y = pos.y;
    m_pos.z = -pos.z;

    update();

    m_hpTimer.reset();
    m_timer = m_hpTimer.time();
}

Camera::~Camera()
{
    delete m_frustum;
	delete m_spline;
	delete m_splineView;
	delete m_splineSpeed;
}



void Camera::setPerspective(Transform &trans)
{
    update();
	
	glm::mat4 view = m_rotMat;
	view = glm::translate(view, glm::vec3(-m_pos.x, -m_pos.y, m_pos.z));
	glm::mat4 projection = glm::perspective(glm::radians(m_fov) , m_aspect, m_ncp, m_fcp);

	trans.projection = projection;
	trans.view = view;
	trans.viewProjection = projection * view;


	//m_fixUp     = glm::normalize(glm::vec3(view.a21, view.a22, view.a23));
	//m_fixRight  = glm::normalize(glm::vec3(view.a11, view.a12, view.a13));
	//m_fixUp = glm::normalize(glm::vec3(view[0][1], view[1][1], view[2][1]));
	//m_fixRight = glm::normalize(glm::vec3(view[0][0], view[1][0], view[2][0]));
	//m_fixDir    = glm::normalize(glm::cross(m_fixUp, m_fixRight));
}

void Camera::determineMovement()
{
    //Frame Independence
    m_timerDiff = m_hpTimer.time() - m_timer;
    m_secsPerFrame = (float)(m_timerDiff);

    m_movementValue = (float)(m_desiredDistance * m_secsPerFrame);
    m_timer = m_hpTimer.time();
}

void Camera::update()
{
	glm::quat pitch = glm::angleAxis(glm::radians(m_pitch), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::quat heading = glm::angleAxis(glm::radians(m_heading), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::quat roll = glm::angleAxis(glm::radians(m_roll), glm::vec3(0.0f, 0.0f, 1.0f));

	m_rotMat = glm::mat4_cast(pitch * heading * roll);

	m_dir.x = -glm::mat4_cast(roll * pitch * heading)[0][2];
	m_dir.y = -glm::mat4_cast(pitch)[1][2];
	m_dir.z = glm::mat4_cast(roll * pitch * heading)[2][2];


	glm::mat4 tmpRot = glm::rotate(glm::mat4(1.0f), glm::radians(m_pitch), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::vec4 test = m_rotMat * glm::vec4(m_up, 1.0f);
	//std::cout << "cam dir: " << glm::to_string(test) << std::endl;

	m_tempDir = m_dir;

	// Scale the direction by our speed.
	m_strafe = cross(m_dir, m_up) * m_strafeSpeed;
	m_dir = m_dir * m_forwardSpeed;

	m_pos += m_dir;
	m_pos += m_strafe;

	m_realPos = glm::vec3(m_pos.x, m_pos.y, -m_pos.z);

	glm::vec3 fruDir = m_tempDir;
	fruDir.x *= -1;
	fruDir.y *= -1;

	m_frustum->setCamDef(m_realPos, fruDir, m_up);
	m_showFrustum->setCamDef(m_realPos, fruDir, m_up);
}

void Camera::changePitch(GLfloat degrees)
{
    m_pitch += degrees;
}

void Camera::changeHeading(GLfloat degrees)
{
    m_heading += degrees;
}

void Camera::changeRoll(GLfloat degrees)
{
    m_roll += degrees;

	if(m_roll > 360.0f)
		m_roll -= 360.0f;
	else if(m_roll < -360.0f)
		m_roll += 360.0f;
}

void Camera::moveForward(bool move)
{
    if(move)
        m_forwardSpeed = m_movementValue;
    else
        m_forwardSpeed = 0.0f;
}

void Camera::moveBackward(bool move)
{
    if(move)
        m_forwardSpeed = -m_movementValue;
    else
        m_forwardSpeed = 0.0f;
}

void Camera::strafeLeft(bool move)
{
    if(move)
        m_strafeSpeed = m_movementValue;
    else
        m_strafeSpeed = 0.0f;
}

void Camera::strafeRight(bool move)
{
    if(move)
        m_strafeSpeed = -m_movementValue;
    else
        m_strafeSpeed = 0.0f;
}

void Camera::increaseDistPerSec(float delta)
{
    m_desiredDistance += delta;
}

void Camera::decreaseDistPerSec(float delta)
{
    m_desiredDistance -= delta;
    if(m_desiredDistance < 0.1f)
        m_desiredDistance = 0.1f;
}

void Camera::setDistPerSec(float value)
{
    m_desiredDistance = value;
}

void Camera::render()
{
 //   glPushAttrib(GL_ALL_ATTRIB_BITS);
 //   glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);

	//auto trans = RenderContext::transform();

 //   glEnableFixedFunction(*trans);

 //       glDisable(GL_TEXTURE_2D);
 //       glColor4f(m_cameraColor.x, m_cameraColor.y, m_cameraColor.z, 1.0);
	//    glPushMatrix();        
 //           glTranslatef(m_realPos.x, m_realPos.y, m_realPos.z);
 //           glutSolidSphere(0.1, 10, 10);
 //       glPopMatrix();

 //       if(m_renderFrustum)		
 //           m_showFrustum->drawLines();

	//    //if(m_showFrustum)		
	//    //	m_showFrustum->drawPlanes();

 //       //renderFrames();
 //       //renderSpline();

 //   glDisableFixedFunction();

 //   glPopClientAttrib();
 //   glPopAttrib();
}

void Camera::renderFrames()
{ 
  //  glColor4f(m_cameraColor.x, m_cameraColor.y, m_cameraColor.z, 1.0f);

  //  glPushMatrix();
  //  glBegin(GL_LINE_STRIP);
  //  for(uint i=0; i<m_camFrames.size(); ++i)
  //  {              
		//glm::vec3 p = m_camFrames.at(i).pos;
  //      glVertex3f(p.x, p.y, -p.z);
  //  }
  //  glEnd();
  //  glPopMatrix();
}

void Camera::renderSpline()
{
  //  if(!m_spline || m_spline->numPoints() == 0)
  //      return;

  //  glPushMatrix();
  //  glPointSize(3.0f);
  //  glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
  //  for(float f=0.0f; f<1; f+= 0.01f)
  //  {
		//glm::vec3 v = m_spline->interpolatedPoint(f);

  //      glBegin(GL_POINTS);
  //          glVertex3f(v.x, v.y, v.z);
  //      glEnd();
  //  }
  //  glPopMatrix(); 
}

glm::vec3 Camera::position() const
{
    return m_realPos;
}

float Camera::heading() const
{
    return m_heading;
}

float Camera::pitch() const
{
    return m_pitch;
}

float Camera::roll() const
{
    return m_roll;
}

glm::vec3 Camera::direction() const
{
	glm::vec3 dir = m_tempDir;
    dir.z *= -1;
    return dir;
}

glm::vec3 Camera::up() const
{
    //return m_up;
    return m_fixUp;
}

glm::vec3 Camera::right() const
{
    return m_fixRight;
}

Frustum *Camera::frustum()
{
    return m_frustum;
}

void Camera::updateCamInternals(float ratio)
{
	m_aspect = ratio;
    m_frustum->setCamInternals(m_fov, m_aspect, m_ncp, m_fcp);
	m_showFrustum->setCamInternals(m_fov, m_aspect, 0.2, m_fcp*0.005f);
}

void Camera::addFrame(glm::vec3 pos, float heading, float pitch, float desiredDistance)
{
    m_spline->addPoint(pos);

	glm::vec3 viewDir(heading, pitch, 0.0f);
    m_splineView->addPoint(viewDir);

	glm::vec3 speed(desiredDistance, 0.0f, 0.0f);
	m_splineSpeed->addPoint(speed);

    CameraFrame frame;
    frame.pos = pos;
    frame.pos.z *= -1;
    frame.headingDeg = heading;
    frame.pitchDeg = pitch;
	frame.desiredDistance = desiredDistance;

    m_camFrames.push_back(frame);
}

void Camera::autoAddFrame()
{
	std::cout << "Camera::autoAddFrame()." << std::endl;

	glm::vec3 m_tmpPos = m_pos;
    m_tmpPos.z *= -1.0f;
    m_spline->addPoint(m_tmpPos);

	glm::vec3 viewDir(m_heading, m_pitch, 0.0f);
    m_splineView->addPoint(viewDir);

	glm::vec3 speed(m_desiredDistance, 0.0f, 0.0f);
	m_splineSpeed->addPoint(speed);


    CameraFrame frame;
    frame.pos = m_pos;
    //frame.pos.z *= -1;
    frame.headingDeg = m_heading;
    frame.pitchDeg = m_pitch;
	frame.desiredDistance = speed.x;

    m_camFrames.push_back(frame);
}

void Camera::splineInterpolation()
{	
	float speed = m_movementValue;

	if(m_spline->numPoints() == 0)
		return;

	if(speed < 0.0001f)
		speed = 0.0001f;

	if(m_time > 1.0f)
	{
		m_time = 0.0f;
	}

	m_time += speed * 0.01;

	glm::vec3 v = m_spline->interpolatedPoint(m_time);
	glm::vec3 d = m_splineView->interpolatedPoint(m_time);
	glm::vec3 s = m_splineSpeed->interpolatedPoint(m_time);

	v.z *= -1;
	m_pos = v;
	m_heading = d.x;
	m_pitch = d.y;
	m_desiredDistance = s.x;
}

void Camera::interpolate(float speed)
{
    if(m_camFrames.size() == 0)
        return;

    if(m_idxInterpolOld >= m_camFrames.size())
        m_idxInterpolOld = 0;

    if(m_idxInterpolNew >= m_camFrames.size())
        m_idxInterpolNew = 0;

    if(m_lerpFactor > 1.0)
        m_lerpFactor = 1.0;

	glm::vec3 oldPos = m_camFrames.at(m_idxInterpolOld).pos;
	glm::vec3 newPos = m_camFrames.at(m_idxInterpolNew).pos;
	glm::vec3 curPos = glm::vec3(0.0, 0.0, 0.0);

    curPos.x = Common::hermiteInterpolation(oldPos.x/4, oldPos.x, newPos.x, newPos.x/2, m_lerpFactor, 1, 1);
    curPos.y = Common::hermiteInterpolation(oldPos.y/4, oldPos.y, newPos.y, newPos.y/2, m_lerpFactor, 1, 1);
    curPos.z = Common::hermiteInterpolation(oldPos.z/4, oldPos.z, newPos.z, newPos.z/2, m_lerpFactor, 1, 1);
    
    m_pos = curPos;

    float oldHeadDeg = m_camFrames.at(m_idxInterpolOld).headingDeg;
    float newHeadDeg = m_camFrames.at(m_idxInterpolNew).headingDeg;
    float curHeadDeg = 0.0f;

    curHeadDeg = Common::hermiteInterpolation(oldHeadDeg/4, oldHeadDeg, newHeadDeg, newHeadDeg/2, m_lerpFactor, 1, 1);

    m_heading = curHeadDeg;

    float oldPitchDeg = m_camFrames.at(m_idxInterpolOld).pitchDeg;
    float newPitchDeg = m_camFrames.at(m_idxInterpolNew).pitchDeg;
    float curPitchDeg = 0.0f;

    curPitchDeg = Common::hermiteInterpolation(oldPitchDeg/4, oldPitchDeg, newPitchDeg, newPitchDeg/2, m_lerpFactor, 1, 1);

    m_pitch = curPitchDeg;


    m_lerpFactor += speed;

    if(curPos == newPos)
    {
        m_idxInterpolOld = m_idxInterpolNew;
        m_idxInterpolNew ++;
        m_lerpFactor = 0.0;
    }

 /*   setPerspective();
    update();*/
}

void Camera::clearFrames()
{
	std::cout << "Camera::clearFrames()." << std::endl;

    m_camFrames.clear();
    m_spline->clear();
    m_splineView->clear();
	m_splineSpeed->clear();
}

void Camera::saveFrames(const std::string &filePath)
{
	std::cout << "Camera::saveFrames(): " << filePath  << std::endl;

	std::ofstream file;
	file.open(filePath);

     for(int i=0; i<m_camFrames.size(); ++i)
     {
         CameraFrame cf = m_camFrames.at(i);
         
         if(i > 0)
			 file << "\n";
         
		 file << cf.pos.x << " " << cf.pos.y << " " << cf.pos.z << " " << cf.headingDeg << " " << cf.pitchDeg << " " << cf.desiredDistance;
     }

     file.close();
}

void Camera::saveFrames()
{
	time_t t = time(0);   // get time now
	struct tm * now = localtime(&t);
	
	int year = now->tm_year - 100;
	int month = now->tm_mon + 1;
	int day = now->tm_mday;

	int hour = now->tm_hour;
	int minute = now->tm_min;
	int second = now->tm_sec;

	std::string sYear   = std::to_string(year);
	std::string sMonth  = std::to_string(month);
	std::string sDay    = std::to_string(day);
	std::string sHour   = std::to_string(hour);
	std::string sMinute = std::to_string(minute);
	std::string sSecond = std::to_string(second);
	std::string sNull   = std::to_string(0);

	std::string  fMonth  = month < 10 ? sNull + sMonth : sMonth;
	std::string  fDay    = day < 10 ? sNull + sDay : sDay;
	std::string  fHour   = hour < 10 ? sNull + sHour : sHour;
	std::string  fMinute = minute < 10 ? sNull + sMinute : sMinute;
	std::string  fSecond = second < 10 ? sNull + sSecond : sSecond;


	std::string fileName = sYear + fMonth + fDay + "_" + fHour + fMinute + fSecond + ".cam";
	std::string filePath = m_frameSetFolder + "/" + fileName;

	saveFrames(filePath);
}

void Camera::loadFrames(const std::string &filePath)
{
	 delete m_spline;
	 m_spline = nullptr;

	 delete m_splineView;
	 m_splineView = nullptr;

	 delete m_splineSpeed;
	 m_splineSpeed = nullptr;

	 m_spline      = new Spline();
	 m_splineView  = new Spline();
	 m_splineSpeed = new Spline();


	 m_camFrames.clear();
	 m_time = 0;

	 std::string line;
	 std::ifstream myfile(filePath);

	 if (myfile.is_open())
	 {
		 while (getline(myfile, line))
		 {
			 glm::vec3 v;
			 float heading = 0.0f;
			 float pitch = 0.0f;
			 float desiredDistance = 0.0f;

			 std::vector<std::string> internal;
			 std::stringstream ss(line); 
			 std::string tok;

			 ss >> v.x >> v.y >> v.z >> heading >> pitch >> desiredDistance;
			 v.z *= -1;
			 addFrame(v, heading, pitch, desiredDistance);

		 }
		 myfile.close();
	 }
}

void Camera::loadFrameDirectory(const std::string &folderPath)
{
	m_frameSetFolder = folderPath;

	std::experimental::filesystem::path dir(folderPath);

	if (!is_directory(dir))
	{
		std::cout << "void Camera::loadFrameDirectory(): " << folderPath << " is no directory!" << std::endl;
	}

	for (std::experimental::filesystem::directory_iterator it(dir); it != std::experimental::filesystem::directory_iterator(); ++it)
	{
		if (is_regular_file(it->status()))
		{		
			std::string fi = it->path().string();

			loadFrames(fi);
			m_activeFrameSetName = fi;
			m_frameFileNames.push_back(fi);	
		}
	}
}

void Camera::changeFrameSet()
{
	m_activeFrameSet++;

	if(m_activeFrameSet >= m_frameFileNames.size())
		m_activeFrameSet = 0;

	if(m_frameFileNames.size() > 0)
	{
		std::string fi = m_frameFileNames.at(m_activeFrameSet);

		std::experimental::filesystem::path file(fi);

		if (is_regular_file(file))
		{
			loadFrames(fi);
			m_activeFrameSetName = fi;
		}
	}
}

std::string Camera::frameSetName() const
{
	return m_activeFrameSetName;
}

void Camera::setPosition(float x, float y, float z)
{
    m_pos.x = x;
    m_pos.y = y;
    m_pos.z = -z;
}

void Camera::setHeading(float degrees)
{
	m_heading = degrees;
}

void Camera::setPitch(float degrees)
{
	m_pitch = degrees;
}

void Camera::setColor(float r, float g, float b)
{
    m_cameraColor.x = r;
    m_cameraColor.y = g;
    m_cameraColor.z = b;
}

void Camera::setSpeed(float s)
{
	m_desiredDistance = s;
}

void Camera::toggleInterpolation()
{
    m_interpolate = !m_interpolate;
}

void Camera::interpolate()
{
    if(m_interpolate)
        splineInterpolation();
}

float Camera::fov() const
{
	return m_fov;
}

float Camera::ncp() const
{
	return m_ncp;
}

float Camera::fcp() const
{
	return m_fcp;
}