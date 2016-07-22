//Author: Julian Kratt
//Date: 22.07.2016

#ifndef GESTALT_MESG_UTILS_H
#define GESTALT_MESG_UTILS_H

#include "MeshData.h"


#include <array>
#include <vector>
#include <map>

namespace utils {

	std::vector<glm::vec3> randomPointSampling(const std::map<unsigned int, MeshFace> &faces, const std::map<unsigned int, MeshVertex> &vertices, float density);
	std::vector<glm::vec3> gridPointSampling(const std::map<unsigned int, MeshFace> &faces, const std::map<unsigned int, MeshVertex> &vertices, float density);

	bool pointInsideTriangle(const glm::vec3 &s, const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c);
	glm::vec3 barycentricCoords(const glm::vec3 &p, const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c);
	//bool ray_mesh_intersection(const glm::vec3 &rayStart, const glm::vec3 &rayDir, const std::shared_ptr<SurfaceMesh> &mesh,  glm::vec3 &intersection );

	std::vector<unsigned int> get_incident_face_ids(const std::map<unsigned int, MeshFace> &faces, const std::map<unsigned int, MeshVertex> &vertices, unsigned int faceId);
	std::vector<unsigned int> get_common_vert_ids(const MeshFace &aFace, const MeshFace &bFace);
	std::vector<std::pair<unsigned int, unsigned int>> get_border_edges(const std::map<unsigned int, MeshFace> &faces, const std::map<unsigned int, MeshVertex> &vertices, unsigned int faceId);

	void set_neighbor_ids(const std::map<unsigned int, MeshFace> &faces, std::map<unsigned int, MeshVertex> &vertices);
	void set_normals(std::map<unsigned int, MeshFace> &faces, std::map<unsigned int, MeshVertex> &vertices);

	void print_mesh_data(const std::map<unsigned int, MeshFace> &faces, const std::map<unsigned int, MeshVertex> &vertices);
}

#endif
