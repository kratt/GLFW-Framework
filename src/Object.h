#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include <vector>
#include <map>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class Shader;
class Texture;
class VertexBufferObject;

struct ObjectMaterial
{
	Texture *texKd;
};

class Object
{
public:
	Object(const std::string &fileName, const glm::vec3 &pos = glm::vec3(0.0f), const glm::vec3 &scale = glm::vec3(1.0f), const glm::vec4 &rot = glm::vec4(0.0f), const glm::vec4 &color = glm:: vec4(1.0f));
	~Object();

	void render();
	void renderDepth();
	void updateAnimation(unsigned int fps);

	bool m_isSelected;
	glm::vec3 m_min;
	glm::vec3 m_max;
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;
	glm::vec4 m_color;
	glm::vec4 m_center;
	glm::vec4 m_up;
	glm::vec4 m_right;
	glm::vec4 m_front;

private:
	void init();
	void initShaders();

	void buildVBOs(const std::string &fileName, const glm::vec3 &rot, const glm::vec3 &scale);
	void buildVBOsAnimation(const std::string &fileName, const glm::vec3 &rot, const glm::vec3 &scale);

private:
	std::string m_fileName;
	std::vector<std::string> m_fileNamesAnimation;

	Shader *m_shaderTriangles;
	Shader *m_shaderTrianglesDepth;
	Shader *m_shaderLines;

	std::vector<VertexBufferObject *> m_vbosTriangles;
	std::vector<VertexBufferObject *> m_vbosLines;
	std::vector<std::string> m_materialNames;

	std::vector< std::vector<VertexBufferObject *> > m_vbosTrianglesAnimation;
	std::vector< std::vector<VertexBufferObject *> > m_vbosLinesAnimation;
	std::vector< std::vector<std::string> > m_materialNamesAnimation;

	std::map<std::string, ObjectMaterial> m_materialCache;

	int m_nrTriangles;
	int m_nrVertices;

	int m_curAnimationIdx;
	float m_curAnimationTime;
	float m_animationSpeed;

	bool m_isAnimated;
	bool m_hasMaterial;
};

#endif