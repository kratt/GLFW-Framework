//Author: Julian Kratt
//Date: 16.11.2015

#include "mesh_utils.h"
#include "Common.h"
#include "log/Log.h"
#include "log/vec.h"
#include <glm/gtx/norm.hpp>

#include <iostream>
#include <string>

namespace utils {

	std::vector<glm::vec3> randomPointSampling(const std::map<unsigned int, MeshFace> &faces, const std::map<unsigned int, MeshVertex> &vertices, float density)
	{
		// sample points within each triangle face
		std::vector<glm::vec3> samplePoints;
		for (auto face : faces)
		{
			const glm::vec3 &va = vertices.find(face.second.a)->second.pos;
			const glm::vec3 &vb = vertices.find(face.second.b)->second.pos;
			const glm::vec3 &vc = vertices.find(face.second.c)->second.pos;

			glm::vec3 vab = vb - va;
			glm::vec3 vac = vc - va;

			float area = length(glm::cross(vab, vac)) * 0.5f;
			int point_count = std::ceil(area / density);

			for (int i = 0; i < point_count; ++i)
			{
				float barycentric_u = frand(0.0f, 1.0f);
				float barycentric_v = frand(0.0f, 1.0f);
				float barycentric_w = frand(0.0f, 1.0f);
				float total = (barycentric_u + barycentric_v + barycentric_w);

				if (total < 0.00001f)
				{
					barycentric_u = 0.0f;
					barycentric_v = 0.0f;
				}
				else
				{
					barycentric_u = barycentric_u / total;
					barycentric_v = barycentric_v / total;
				}


				glm::vec3 sample = (vab * barycentric_u) + (vac * barycentric_v) + va;
				samplePoints.push_back(sample);
			}
		}

		return samplePoints;
	}

	std::vector<glm::vec3> gridPointSampling(const std::map<unsigned int, MeshFace> &faces, const std::map<unsigned int, MeshVertex> &vertices, float density)
	{
		float eps = 0.00001f;
		std::vector<glm::vec3> samplePoints;
		for (auto face : faces)
		{
			const glm::vec3 &va = vertices.find(face.second.a)->second.pos;
			const glm::vec3 &vb = vertices.find(face.second.b)->second.pos;
			const glm::vec3 &vc = vertices.find(face.second.c)->second.pos;

			glm::vec3 vab = vb - va;
			glm::vec3 vac = vc - va;

			if (glm::length(vab) < eps || glm::length(vac) < eps)
				continue;

			glm::vec3 normal = glm::cross(vab, vac);

			// local coordinate frame of triangle
			glm::vec3 u = normalize(vab);
			glm::vec3 v = normalize(cross(normal, u));

			// do we have to flip v?
			if (glm::dot(v, vac) < 0.0f)
				v *= 1.0f;

			// determine 
			float max_len = std::max(glm::length(vab), glm::length(vac));
			float sample_step = density;


			// sample grid points
			for (float i = 0.0f; i < max_len; i += sample_step)
			{
				for (float j = 0.0f; j < max_len; j += sample_step)
				{
					glm::vec3 sample = va + i*u + j*v;

					// point inside triangle?
					if (pointInsideTriangle(sample, va, vb, vc))
						samplePoints.push_back(sample);
				}
			}
		}

		return samplePoints;
	}


	bool pointInsideTriangle(const glm::vec3 &p, const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c)
	{
		glm::vec3 uvw = barycentricCoords(p, a, b, c);

		if (uvw.x <= 0 || uvw.y <= 0 || uvw.z <= 0)
			return false;
		else
			return true;
	}

	bool intersect_triangle(const glm::vec3 &rayStart, const glm::vec3 &rayDir, const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2, float &t)
	{
		float eps = 0.000000001f;

		glm::vec3 e1 = v1 - v0;
		glm::vec3 e2 = v2 - v0;

		glm::vec3 p = cross(rayDir, e2);
		float a = dot(e1, p);

		if (a > -eps && a < eps)
		{
			return false;
		}

		float f = 1.0f / a;
		glm::vec3 s = rayStart - v0;
		float u = f * dot(s, p);

		if (u < 0.0f || u > 1.0f)
			return false;

		glm::vec3 q = cross(s, e1);
		float v = f * dot(rayDir, q);

		if (v < 0.0f || u + v > 1.0f)
			return false;

		t = f * dot(e2, q);

		return true;
	}

	glm::vec3 barycentricCoords(const glm::vec3 &p, const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c)
	{
		glm::vec3 v0 = b - a;
		glm::vec3 v1 = c - a;
		glm::vec3 v2 = p - a;

		float d00 = glm::dot(v0, v0);
		float d01 = glm::dot(v0, v1);
		float d11 = glm::dot(v1, v1);
		float d20 = glm::dot(v2, v0);
		float d21 = glm::dot(v2, v1);
		float denom = d00 * d11 - d01 * d01;

		float v = (d11 * d20 - d01 * d21) / denom;
		float w = (d00 * d21 - d01 * d20) / denom;
		float u = 1.0f - v - w;

		return glm::vec3(u, v, w);
	}

	std::vector<unsigned int> get_incident_face_ids(const std::map<unsigned int, MeshFace> &faces, const std::map<unsigned int, MeshVertex> &vertices, unsigned int faceId)
	{
		std::vector<unsigned int> resIds;

		auto iterFace = faces.find(faceId);
		if (iterFace == faces.end())
			return resIds;

		const MeshFace &face = iterFace->second;

		auto iterVertA = vertices.find(face.a);
		auto iterVertB = vertices.find(face.b);
		auto iterVertC = vertices.find(face.c);

		if (iterVertA == vertices.end() ||
			iterVertB == vertices.end() ||
			iterVertC == vertices.end())
		{
			debugLog() << "Vertex not found!" << std::endl;
			return resIds;
		}

		const MeshVertex &va = iterVertA->second;
		const MeshVertex &vb = iterVertB->second;
		const MeshVertex &vc = iterVertC->second;


		// edge a-b
		for (unsigned int id : va.faceIds)
		{
			if (face.id != id && std::find(vb.faceIds.begin(), vb.faceIds.end(), id) != vb.faceIds.end() && std::find(resIds.begin(), resIds.end(), id) == resIds.end())
			{
				resIds.push_back(id);
			}
		}

		// edge b-c
		for (unsigned int id : vb.faceIds)
		{
			if (face.id != id && std::find(vc.faceIds.begin(), vc.faceIds.end(), id) != vc.faceIds.end() && std::find(resIds.begin(), resIds.end(), id) == resIds.end())
			{
				resIds.push_back(id);
			}
		}

		// edge c-a
		for (unsigned int id : vc.faceIds)
		{
			if (face.id != id && std::find(va.faceIds.begin(), va.faceIds.end(), id) != va.faceIds.end() && std::find(resIds.begin(), resIds.end(), id) == resIds.end())
			{
				resIds.push_back(id);
			}
		}


		return resIds;
	}

	void set_neighbor_ids(const std::map<unsigned int, MeshFace> &faces, std::map<unsigned int, MeshVertex> &vertices)
	{
		// set face ids for each vertex
		for (auto &face : faces)
		{
			auto iterVertA = vertices.find(face.second.a);
			auto iterVertB = vertices.find(face.second.b);
			auto iterVertC = vertices.find(face.second.c);

			if (iterVertA == vertices.end() ||
				iterVertB == vertices.end() ||
				iterVertC == vertices.end())
			{
				debugLog() << "Vertex not found!" << std::endl;
			}

			MeshVertex &va = iterVertA->second;
			MeshVertex &vb = iterVertB->second;
			MeshVertex &vc = iterVertC->second;

			va.faceIds.push_back(face.second.id);
			vb.faceIds.push_back(face.second.id);
			vc.faceIds.push_back(face.second.id);
		}
	}

	void set_normals(std::map<unsigned int, MeshFace> &faces, std::map<unsigned int, MeshVertex> &vertices)
	{
		// set face normals
		for (auto &face : faces)
		{
			auto iterVertA = vertices.find(face.second.a);
			auto iterVertB = vertices.find(face.second.b);
			auto iterVertC = vertices.find(face.second.c);

			if (iterVertA == vertices.end() ||
				iterVertB == vertices.end() ||
				iterVertC == vertices.end())
			{
				debugLog() << "vertex not found!" << std::endl;
			}

			MeshVertex &va = iterVertA->second;
			MeshVertex &vb = iterVertB->second;
			MeshVertex &vc = iterVertC->second;

			face.second.normal = glm::normalize(glm::cross(vb.pos - va.pos, vc.pos - va.pos));
		}

		// set avg. vert normals
		for (auto &vert : vertices)
		{
			auto faceIds = vert.second.faceIds;

			int count = 0;
			vert.second.normal = glm::vec3(0.0f, 0.0f, 0.0f);
			for (auto id : faceIds)
			{
				auto iterFace = faces.find(id);

				if (iterFace != faces.end())
				{
					vert.second.normal += iterFace->second.normal;
					++count;
				}
			}

			if (count > 0)
				vert.second.normal /= count;
		}
	}


	std::vector<unsigned int> get_common_vert_ids(const MeshFace &aFace, const MeshFace &bFace)
	{
		std::vector<unsigned int> ids_a;
		ids_a.push_back(aFace.a);
		ids_a.push_back(aFace.b);
		ids_a.push_back(aFace.c);

		std::vector<unsigned int> ids_b;
		ids_b.push_back(bFace.a);
		ids_b.push_back(bFace.b);
		ids_b.push_back(bFace.c);

		std::vector<unsigned int> common_ids;
		for (unsigned int a : ids_a)
		{
			if (std::find(ids_b.begin(), ids_b.end(), a) != ids_b.end())
			{
				common_ids.push_back(a);
			}
		}

		return common_ids;
	}

	std::vector<std::pair<unsigned int, unsigned int>> get_border_edges(const std::map<unsigned int, MeshFace> &faces, const std::map<unsigned int, MeshVertex> &vertices, unsigned int faceId)
	{
		std::vector<std::pair<unsigned int, unsigned int>> resEdges;

		// first get incident faces
		auto incidentFaceIds = get_incident_face_ids(faces, vertices, faceId);

		auto iterFace = faces.find(faceId);
		if (iterFace == faces.end())
			return resEdges;

		MeshFace face = iterFace->second;

		// collect all edges that have an incident edges
		std::vector<std::pair<unsigned int, unsigned int>> inc_edges;
		for (auto id : incidentFaceIds)
		{
			MeshFace incFace = faces.find(id)->second;
			auto common_ids = get_common_vert_ids(face, incFace);

			if (common_ids.size() == 2)
			{
				inc_edges.push_back(std::make_pair(common_ids[0], common_ids[1]));
			}
		}

		unsigned int a = iterFace->second.a;
		unsigned int b = iterFace->second.b;
		unsigned int c = iterFace->second.c;

		auto edge_ab = std::make_pair(a, b);
		auto edge_bc = std::make_pair(b, c);
		auto edge_ca = std::make_pair(c, a);

		// not so pretty :/ (change this later!)
		auto iterAB = std::find_if(inc_edges.begin(), inc_edges.end(), [&edge_ab](const std::pair<unsigned int, unsigned int> &e)
		{
			return (e.first == edge_ab.first  && e.second == edge_ab.second) ||
				(e.first == edge_ab.second && e.second == edge_ab.first);
		});

		auto iterBC = std::find_if(inc_edges.begin(), inc_edges.end(), [&edge_bc](const std::pair<unsigned int, unsigned int> &e)
		{
			return (e.first == edge_bc.first  && e.second == edge_bc.second) ||
				(e.first == edge_bc.second && e.second == edge_bc.first);
		});

		auto iterCA = std::find_if(inc_edges.begin(), inc_edges.end(), [&edge_ca](const std::pair<unsigned int, unsigned int> &e)
		{
			return (e.first == edge_ca.first  && e.second == edge_ca.second) ||
				(e.first == edge_ca.second && e.second == edge_ca.first);
		});

		if (iterAB == inc_edges.end())
			resEdges.push_back(edge_ab);

		if (iterBC == inc_edges.end())
			resEdges.push_back(edge_bc);

		if (iterCA == inc_edges.end())
			resEdges.push_back(edge_ca);

		return resEdges;
	}


	void print_mesh_data(const std::map<unsigned int, MeshFace> &faces, const std::map<unsigned int, MeshVertex> &vertices)
	{
		debugLog() << "Mesh data: #faces:" << faces.size() << ", #vertices:" << vertices.size() << std::endl;
		debugLog() << "Vertices:" << std::endl;

		for (auto vert : vertices)
		{
			std::string res;
			for (auto id : vert.second.faceIds)
			{
				res = res + std::to_string(id) + ", ";
			}

			debugLog() << "vert: " << vert.second.id << ", " << vert.second.pos << "face ids: " << res << std::endl;
		}

		debugLog() << std::endl;
		debugLog() << "Faces:" << std::endl;

		for (auto face : faces)
		{
			debugLog() << "face: " << face.second.id << "(" << face.second.a << ", " << face.second.b << ", " << face.second.c << ")" << std::endl;
		}
	}

}
