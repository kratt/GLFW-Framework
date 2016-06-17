//Author: Sören Pirk
//Date: 22.01.2013

#ifndef FRUSTUM
#define FRUSTUM

#include <glm/vec3.hpp>

class Plane  
{

public:

	glm::vec3 normal, point;
	float d;

	Plane(glm::vec3 &v1, glm::vec3 &v2, glm::vec3 &v3);
	Plane(void);
	~Plane();

	void set3Points(glm::vec3 &v1, glm::vec3 &v2, glm::vec3 &v3);
	void setNormalAndPoint(glm::vec3 &normal, glm::vec3 &point);
	void setCoefficients(float a, float b, float c, float d);
	float distance(glm::vec3 p);

	void print();
};

class Frustum 
{
private:

	enum {
		TOP = 0,
		BOTTOM,
		LEFT,
		RIGHT,
		NEARP,
		FARP
	};

    float m_angle;

public:

	static enum {OUTSIDE, INTERSECT, INSIDE};

	Plane pl[6];


	glm::vec3 ntl, ntr, nbl, nbr, ftl, ftr, fbl, fbr;
	float nearD, farD, ratio, angle,tang;
	float nw,nh,fw,fh;

	Frustum();
	~Frustum();

	void setCamInternals(float angle, float ratio, float nearD, float farD);
	void setCamDef(glm::vec3 &p, glm::vec3 &l, glm::vec3 &u);
	int pointInFrustum(glm::vec3 &p);
	int sphereInFrustum(glm::vec3 &p, float raio);
	//int boxInFrustum(AABox &b);
    int boxInFrustum(glm::vec3 &min, glm::vec3 &max);

    glm::vec3 getVertexN(glm::vec3 &normal, glm::vec3 &min, glm::vec3 &max);
    glm::vec3 getVertexP(glm::vec3 &normal, glm::vec3 &min, glm::vec3 &max);

	//void drawPoints();
	//void drawLines();
	//void drawPlanes();
	//void drawNormals();
};


#endif