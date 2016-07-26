#ifndef MESH
#define MESH

#include "Global.h"
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class VertexBufferObject;

class Mesh
{
public:
   Mesh();
   ~Mesh();

   static std::vector<VertexBufferObject *> Mesh::obj(const std::string &fileName, const glm::vec3 &rot = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3 &scale = glm::vec3(1.0f, 1.0f, 1.0f), GLenum primitive = GL_TRIANGLES);
   static VertexBufferObject *quadLines(int startX, int startY, int width, int height, const glm::vec4 &color = glm::vec4());
   static VertexBufferObject *quad(int startX, int startY, int width, int height, const glm::vec4 &color = glm::vec4(), GLenum primitive = GL_QUADS);
   static VertexBufferObject *quad(int width, int height, const glm::vec4 &color = glm::vec4(), GLenum primitive = GL_QUADS);
   static VertexBufferObject *sphere(float radius, int iterations, const glm::vec4 &color = glm::vec4(), GLenum primitive = GL_TRIANGLES);
   static VertexBufferObject *box(const glm::vec3 &mi, const glm::vec3 &ma, const glm::vec4 &color = glm::vec4(), GLenum primitive = GL_QUADS);

private:
    typedef struct 
    {
       double x,y,z;
    } XYZ;
    
    typedef struct 
    {
		glm::vec3 p1, p2, p3;
    } FACET3;


    //Paul Bourke Sphere http://paulbourke.net/miscellaneous/sphere_cylinder/
    static int createNSphere(FACET3 *f, int iterations);
    static int CreateUnitSphere(FACET3 *facets, int iterations);
	static glm::vec3 MidPoint(glm::vec3 p1, glm::vec3 p2);

};

#endif

