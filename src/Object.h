#ifndef OBJECT
#define OBJECT

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <string>
#include <vector>

class Shader;
class VertexBufferObjectAttribs;

class Object
{
public:
    Object(const std::string &fileName, const glm::vec3 &pos = glm::vec3(), const glm::vec3 &scale = glm::vec3(1, 1, 1), const glm::vec4 &rot = glm::vec4(), const glm::vec4 &color = glm::vec4(1, 1, 1, 1));
    ~Object();

    void render();
    void renderDepth();

    bool m_isSelected;
    glm::vec3 m_min;
    glm::vec3 m_max;  
    glm::vec3 m_position;
    glm::vec3 m_scale;

	glm::vec4 m_rotation;
    glm::vec4 m_color;
    glm::vec4 m_center;
    glm::vec4 m_up;
    glm::vec4 m_right;
    glm::vec4 m_front;

private:
    void init();
    void buildVBOs(const std::string &fileName, const glm::vec3 &rot, const glm::vec3 &scale);

private:
    std::string m_fileName;
    Shader *m_shaderTriangles;
    Shader *m_shaderTrianglesDepth;
    Shader *m_shaderLines;   

    std::vector<VertexBufferObjectAttribs *> m_vbosTriangles;
    std::vector<VertexBufferObjectAttribs *> m_vbosLines;

    int m_nrTriangles;
    int m_nrVertices;
};

#endif