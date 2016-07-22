//Author: Julian Kratt
//Date: 22.07.2016

#ifndef GESTALT_MESH_DATA_H
#define GESTALT_MESH_DATA_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector>


	struct MeshFace
	{
		unsigned int a, b, c, id;
		glm::vec3 normal, center;
	};

	struct MeshVertex
	{
		unsigned int id;
		glm::vec3 pos, normal, texCoords;
		glm::vec4 color;
		std::vector<unsigned int> faceIds, vertIds;
	};

	struct Segment2D
	{
		glm::vec2 s;
		glm::vec2 t;

		Segment2D(){};
		Segment2D(glm::vec2 a, glm::vec2 b)
			: s(a), t(b){};
	};

	struct Segment3D
	{
		glm::vec3 s;
		glm::vec3 t;

		Segment3D(){};
		Segment3D(glm::vec3 a, glm::vec3 b)
			: s(a), t(b){};
	};

	struct Path2D
	{
		std::vector<glm::vec2> points;
	};

	struct ObjFace
	{
		unsigned int v0, t0, n0;
		unsigned int v1, t1, n1;
		unsigned int v2, t2, n2;
		unsigned int v3, t3, n3;
	};


#endif
