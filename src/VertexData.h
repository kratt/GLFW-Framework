// Computer Graphics and Media Informatics
// University of Konstanz
// 25.07.2016
// author: Julian Kratt

#ifndef VERTEX_DATA_H
#define VERTEX_DATA_H

#include "Global.h"

struct VertexData
{
	GLfloat vx, vy, vz, vw; // Position Coordinates
	GLfloat nx, ny, nz, nw; // Normal
	GLfloat cx, cy, cz, cw; // Color
	GLfloat tx, ty, tz, tw; // Texture Coordinates
};


#endif

