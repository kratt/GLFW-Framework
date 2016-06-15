// Computer Graphics and Media Informatics
// University of Konstanz
// 2015

#include "ObjLoader.h"
#include "VertexBufferObjectAttribs.h"
#include "ModelLoaderObj.h"
#include <fstream>

/**
	Creates a vertex buffer object (geometry storage) from the path of an *obj file.
*/
VertexBufferObjectAttribs* ObjLoader::createVertexBufferObject(char* file)
{
    ModelOBJ *model = new ModelOBJ();	
	model->import(file);
    
    int nrVertices = model->getNumberOfVertices();

    const ModelOBJ::Vertex *vb = model->getVertexBuffer();
    const int *ib = model->getIndexBuffer();
    
	std::vector<glm::vec3> tmpVertices;	
	std::vector<glm::vec3> tmpNormals;
	std::vector<glm::vec3> tmpTexCoords;

	const ModelOBJ::Mesh &objMesh = model->getMesh(0);
	const int startIndex = objMesh.startIndex;
	const int vertexCount = objMesh.triangleCount * 3;

    for(int j=0; j<nrVertices; ++j)
	{
        ModelOBJ::Vertex v = vb[j];

		glm::vec3 p(v.position[0], v.position[1], v.position[2]);
		glm::vec3 n(v.normal[0], v.normal[1], v.normal[2]);
		glm::vec3 t(v.texCoord[0], v.texCoord[1], 0.0f);  
			
		tmpVertices.push_back(p);
		tmpNormals.push_back(n);
		tmpTexCoords.push_back(t);
    }        

	VertexBufferObjectAttribs::DATA *data = new VertexBufferObjectAttribs::DATA[tmpVertices.size()];

	for(unsigned int i=0; i<tmpVertices.size(); ++i)
	{
		glm::vec3 pos = tmpVertices[i];
		glm::vec3 n   = tmpNormals[i];
		glm::vec3 t   = tmpTexCoords[i];

		data[i].vx = pos.x;
		data[i].vy = pos.y;
		data[i].vz = pos.z;
		data[i].vw = 1.0f;

		data[i].cx = 0.0f;
		data[i].cy = 0.0f;
		data[i].cz = 0.0f;
		data[i].cw = 1.0f;

		data[i].nx = n.x;
		data[i].ny = n.y;
		data[i].nz = n.z;
		data[i].nw = 1.0f;

		data[i].tx = t.x;
		data[i].ty = t.y;
        data[i].tz = 0.0f;
        data[i].tw = 0.0f;
	}

	VertexBufferObjectAttribs* vboMesh = new VertexBufferObjectAttribs();
	vboMesh->setData(data, GL_STATIC_DRAW, tmpVertices.size(), GL_TRIANGLES); 
    vboMesh->setIndexData(ib, GL_STATIC_DRAW, startIndex + vertexCount);

	vboMesh->addAttrib(VERTEX_POSITION);
	vboMesh->addAttrib(VERTEX_NORMAL);
	vboMesh->addAttrib(VERTEX_COLOR);
	vboMesh->addAttrib(VERTEX_TEXTURE);
	vboMesh->bindAttribs();

	delete[] data;

	delete model;

	return vboMesh;
}