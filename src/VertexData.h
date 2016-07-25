// Computer Graphics and Media Informatics
// University of Konstanz
// 25.07.2016
// author: Julian Kratt

#ifndef VERTEX_DATA_H
#define VERTEX_DATA_H

#include "Global.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

struct VertexData
{
	GLfloat vx, vy, vz, vw; // Position Coordinates
	GLfloat nx, ny, nz, nw; // Normal
	GLfloat cx, cy, cz, cw; // Color
	GLfloat tx, ty, tz, tw; // Texture Coordinates


	VertexData(glm::vec4 pos, glm::vec4 normal, glm::vec4 color, glm::vec4 texCoords)
		: vx(pos.x), vy(pos.y), vz(pos.z), vw(pos.w),
		  nx(normal.x), ny(normal.y), nz(normal.z), nw(normal.w),
		  cx(color.x), cy(color.y), cz(color.z), cw(color.w),
		  tx(texCoords.x), ty(texCoords.y), tz(texCoords.z), tw(texCoords.w)
		{};

	VertexData(glm::vec3 pos, glm::vec3 normal, glm::vec3 color, glm::vec3 texCoords)
		: vx(pos.x), vy(pos.y), vz(pos.z), vw(0.0f),
		nx(normal.x), ny(normal.y), nz(normal.z), nw(0.0f),
		cx(color.x), cy(color.y), cz(color.z), cw(0.0f),
		tx(texCoords.x), ty(texCoords.y), tz(texCoords.z), tw(0.0f)
	{};

	VertexData()
	  : vx(0.0f), vy(0.0f), vz(0.0f), vw(0.0f),
		nx(0.0f), ny(0.0f), nz(0.0f), nw(0.0f),
		cx(0.0f), cy(0.0f), cz(0.0f), cw(0.0f),
		tx(0.0f), ty(0.0f), tz(0.0f), tw(0.0f)
	{};
};


#endif

