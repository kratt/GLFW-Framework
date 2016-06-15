#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/mat4x4.hpp>

struct Transform
{
	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 viewProjection;

	glm::mat4 lightView;
	glm::mat4 lightProjection;
	glm::mat4 lightViewProjection;
	glm::mat4 lightViewProjectionBias;

	glm::mat4 normal;
};

#endif
