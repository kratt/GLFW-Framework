#ifndef GLOBAL_OBJECT_PARAM_H
#define GLOBAL_OBJECT_PARAM_H

#include <glm/vec3.hpp> 
#include <glm/vec4.hpp> 

#include "Global.h"

struct GlobalObjectParam
{
	glm::vec3 lightPos;
	glm::vec3 lightDir;
	glm::vec3 camPos;
	float shadowIntensity;
	bool applyShadow;
	int gridRenderMode;
	int polygonMode;
	bool renderMesh;
	bool renderObjects;
	float ncp;
	float fcp;
	float fov;

	float depthRangeMax;
	float depthRangeMin;
	float polygonOffsetFactor;
	float polygonOffsetUnits;

	glm::vec4 clipPlaneGround;

	GLuint shadowMapID;
	GLuint shadowMapBlurredID;

	int windowWidth;
	int windowHeight;
};

#endif
