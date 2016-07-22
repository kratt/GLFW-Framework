#include "Object.h"
#include "Shader.h"
#include "VertexBufferObjectAttribs.h"
#include "ModelLoaderObj.h"
#include "Texture.h"
#include "RenderContext.h"
#include "log\Log.h"

#include <regex> 

Object::Object(const std::string &fileName, const glm::vec3 &pos, const glm::vec3 &scale, const glm::vec4 &rot, const glm::vec4 &color)
	: m_fileName(fileName),
	m_position(pos),
	m_scale(scale),
	m_rotation(rot),
	m_color(color),
	m_shaderTriangles(NULL),
	m_shaderLines(NULL),
	m_shaderTrianglesDepth(NULL),
	m_isSelected(false),
	m_max(math_minfloat, math_minfloat, math_minfloat),
	m_min(math_maxfloat, math_maxfloat, math_maxfloat),
	m_nrTriangles(0),
	m_nrVertices(0),
	m_curAnimationIdx(0),
	m_up(0, 1, 0, 1),
	m_isAnimated(false),
	m_curAnimationTime(0.0f),
	m_animationSpeed(22.0f)
{
	init();
	initShaders();
}

Object::~Object()
{
	delete m_shaderTriangles;
	delete m_shaderLines;
	delete m_shaderTrianglesDepth;

	for (int i = m_vbosTriangles.size() - 1; i >= 0; --i)
	{
		VertexBufferObjectAttribs *vbo = m_vbosTriangles[i];
		delete vbo;
	}

	for (int i = m_vbosLines.size() - 1; i >= 0; --i)
	{
		VertexBufferObjectAttribs *vbo = m_vbosLines[i];
		delete vbo;
	}
}

void Object::init()
{
	std::experimental::filesystem::path file(m_fileName);

	if (is_directory(file))
	{
		debugLog() << "Object::init(): Load Animation: " << m_fileName << std::endl;
		buildVBOsAnimation(m_fileName, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		m_isAnimated = true;
	}
	else 
	{
		debugLog() << "Object::init(): Load Single Object: " << m_fileName << std::endl;
		buildVBOs(m_fileName, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		m_isAnimated = false;
	}
}

void Object::initShaders()
{
	m_shaderTriangles = new Shader("../Shader/Object.vert.glsl", "../Shader/Object.frag.glsl");
	m_shaderTriangles->bindAttribLocations();

	m_shaderTrianglesDepth = new Shader("../Shader/ObjectDepth.vert.glsl", "../Shader/ObjectDepth.frag.glsl");
	m_shaderTrianglesDepth->bindAttribLocations();

	m_shaderLines = new Shader("../Shader/ObjectLines.vert.glsl", "../Shader/ObjectLines.frag.glsl");
	m_shaderLines->bindAttribLocations();
}

void Object::render()
{
	auto param = RenderContext::globalObjectParam();
	auto trans = RenderContext::transform();

	glm::mat4 model = glm::translate(glm::mat4(1.0f), m_position);
	model = glm::rotate(model, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, m_scale);

	glm::mat4 view = trans->view;
	glm::mat4 projection = trans->projection;
	glm::mat4 lightView = trans->lightView;

	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);


	m_shaderTriangles->bind();

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, param->shadowMapID);
	m_shaderTriangles->seti("shadowMap", 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, param->shadowMapBlurredID);
	m_shaderTriangles->seti("shadowMapBlurred", 2);

	m_shaderTriangles->setMatrix("matModel", model, GL_TRUE);
	m_shaderTriangles->setMatrix("matView", view, GL_TRUE);
	m_shaderTriangles->setMatrix("matProjection", projection, GL_TRUE);
	m_shaderTriangles->setMatrix("matLightView", lightView, GL_TRUE);

	m_shaderTriangles->set3f("lightPos", param->lightPos);
	m_shaderTriangles->set3f("camPos", param->camPos);
	m_shaderTriangles->seti("applyShadow", param->applyShadow);
	m_shaderTriangles->setf("shadowIntensity", param->shadowIntensity);
	m_shaderTriangles->seti("isSelected", m_isSelected);


	if (m_isAnimated)
	{
		std::vector<VertexBufferObjectAttribs*> &curVBO = m_vbosTrianglesAnimation[m_curAnimationIdx];
		std::vector<std::string> &matNames = m_materialNamesAnimation[m_curAnimationIdx];

		for (int i = 0; i<curVBO.size(); ++i)
		{
			if (m_hasMaterial)
			{
				ObjectMaterial &objMat = m_materialCache.find(matNames[i])->second;
				Texture *texKd = objMat.texKd;

				glActiveTexture(GL_TEXTURE3);
				glBindTexture(GL_TEXTURE_2D, texKd->id());
				m_shaderTriangles->seti("texKd", 3);
			}

			curVBO[i]->render();
		}
	}
	else
	{
		for (int i = 0; i<m_vbosTriangles.size(); ++i)
		{
			if (m_hasMaterial)
			{
				std::string curMatName = m_materialNames[i];
				ObjectMaterial &objMat = m_materialCache.find(curMatName)->second;
				Texture *texKd = objMat.texKd;

				glActiveTexture(GL_TEXTURE3);
				glBindTexture(GL_TEXTURE_2D, texKd->id());
				m_shaderTriangles->seti("texKd", 3);
			}

			m_vbosTriangles[i]->render();
		}
	}
	m_shaderTriangles->release();

	if (param->renderMesh)
	{
		m_shaderLines->bind();

		m_shaderLines->setMatrix("matModel", model, GL_TRUE);
		m_shaderLines->setMatrix("matView", view, GL_TRUE);
		m_shaderLines->setMatrix("matProjection", projection, GL_TRUE);

		if (m_isAnimated)
		{
			std::vector<VertexBufferObjectAttribs*> &curVBO = m_vbosLinesAnimation[m_curAnimationIdx];

			for (int i = 0; i<curVBO.size(); ++i)
			{
				curVBO[i]->render();
			}
		}
		else
		{
			for (int i = 0; i<m_vbosTriangles.size(); ++i)
			{
				m_vbosLines[i]->render();
			}
		}

		m_shaderLines->release();
	}
}

void Object::renderDepth()
{
	//glEnable(GL_CLIP_DISTANCE0);

	//mat4 model = mat4::translate(m_position) * mat4::rotateY(m_rotation.y) * mat4::scale(m_scale);
	//mat4 view = trans.view;
	//mat4 projection = trans.projection;

	//glDisable(GL_CULL_FACE);
	////glCullFace(GL_FRONT);
	////glFrontFace(GL_CCW);

	////glClearDepth(1.0);
	////glEnable(GL_POLYGON_OFFSET_FILL);
	////glDepthFunc(GL_LEQUAL);
	////glDepthRange(param.depthRangeMin, param.depthRangeMax);
	////glPolygonOffset(param.polygonOffsetFactor, param.polygonOffsetUnits);

	//m_shaderTrianglesDepth->bind();

	//m_shaderTrianglesDepth->setMatrix("matModel", model, GL_TRUE);
	//m_shaderTrianglesDepth->setMatrix("matView", view, GL_TRUE);
	//m_shaderTrianglesDepth->setMatrix("matProjection", projection, GL_TRUE);

	//m_shaderTrianglesDepth->set4f("clipPlane", param.clipPlaneGround);

	//for (uint i = 0; i<m_vbosTriangles.size(); ++i)
	//{
	//	m_vbosTriangles[i]->render();
	//}

	//m_shaderTrianglesDepth->release();

	//glPopClientAttrib();
	//glPopAttrib();
}


void Object::buildVBOsAnimation(const std::string &fileName, const glm::vec3 &rot, const glm::vec3 &scale)
{
	//QDir myDir(fileName);
	//myDir.setNameFilters(QStringList() << "*.obj");
	//QStringList filesList = myDir.entryList(QDir::Files);

	//// first determe scaling factor based on first obj and materials
	//float objScale = 1.0f;
	//vec3 refCenter = vec3(0.0f, 0.0f, 0.0f);

	//if (!filesList.empty())
	//{
	//	ModelOBJ *model = new ModelOBJ();
	//	model->import((QString(fileName) + filesList[0]).toAscii());

	//	uint nrMaterials = model->getNumberOfMaterials();

	//	for (int i = 0; i<nrMaterials; ++i)
	//	{
	//		ModelOBJ::Material mat = model->getMaterial(i);

	//		QString path = m_fileName;
	//		QString colorMapName = QString(mat.colorMapFilename.c_str());
	//		QString tmp = colorMapName.replace(QRegExp("\\\\"), "/");
	//		path.append(tmp);

	//		ObjectMaterial info;
	//		info.texKd = new Texture(path);

	//		m_materialCache.insert(make_pair(mat.name, info));

	//		qDebug() << QString(mat.name.c_str()) << path;
	//	}

	//	if (!m_materialCache.empty())
	//		m_hasMaterial = true;

	//	uint nrMeshes = model->getNumberOfMeshes();
	//	const ModelOBJ::Vertex *vb = model->getVertexBuffer();
	//	const int *tempIdx = model->getIndexBuffer();

	//	vec3 mi(math_maxfloat, math_maxfloat, math_maxfloat);
	//	vec3 ma(math_minfloat, math_minfloat, math_minfloat);

	//	for (uint i = 0; i < nrMeshes; ++i)
	//	{
	//		vector<vec3> tmpVertices;

	//		const ModelOBJ::Mesh &objMesh = model->getMesh(i);

	//		int startIndex = objMesh.startIndex;
	//		int nrVertices = objMesh.triangleCount * 3;

	//		for (int j = startIndex; j<startIndex + nrVertices; ++j)
	//		{
	//			uint idx = tempIdx[j];

	//			vec3 v = vec3(vb[idx].position[0], vb[idx].position[1], vb[idx].position[2]);

	//			if (v.x > ma.x)
	//				ma.x = v.x;

	//			if (v.y > ma.y)
	//				ma.y = v.y;

	//			if (v.z > ma.z)
	//				ma.z = v.z;

	//			if (v.x < mi.x)
	//				mi.x = v.x;

	//			if (v.y < mi.y)
	//				mi.y = v.y;

	//			if (v.z < mi.z)
	//				mi.z = v.z;
	//		}
	//	}

	//	vec3 dist = ma - mi;

	//	if (dist.x > dist.y && dist.x > dist.z) objScale = 1.0f / dist.x;
	//	if (dist.y > dist.x && dist.y > dist.z) objScale = 1.0f / dist.y;
	//	if (dist.z > dist.x && dist.z > dist.y) objScale = 1.0f / dist.z;

	//	objScale *= 1.0f; // optional
	//	refCenter = (mi + ma) * 0.5f;

	//	delete model;
	//}

	//for (int i = 0; i<filesList.size(); ++i)
	//{
	//	std::vector<VertexBufferObjectAttribs*> vbosMesh;
	//	std::vector<VertexBufferObjectAttribs*> vbosLines;
	//	std::vector<std::string> matNames;

	//	ModelOBJ *model = new ModelOBJ();

	//	qDebug() << "OBSTACLEOBJ::load():" << (QString(fileName) + filesList[i]).toAscii();
	//	model->import((QString(fileName) + filesList[i]).toAscii());


	//	uint nrMeshes = model->getNumberOfMeshes();

	//	const ModelOBJ::Vertex *vb = model->getVertexBuffer();
	//	const int *tempIdx = model->getIndexBuffer();

	//	mat4 rotMatX = mat4::rotate(rot.x, 1.0f, 0.0f, 0.0f);
	//	mat4 rotMatY = mat4::rotate(rot.y, 0.0f, 1.0f, 0.0f);
	//	mat4 rotMatZ = mat4::rotate(rot.z, 0.0f, 0.0f, 1.0f);
	//	mat4 scaleMat = mat4::scale(scale.x, scale.y, scale.z);

	//	for (uint i = 0; i < nrMeshes; ++i)
	//	{
	//		vector<vec3> tmpVertices;
	//		vector<vec3> tmpNormals;
	//		vector<vec3> tmpTexCoords;

	//		const ModelOBJ::Mesh &objMesh = model->getMesh(i);
	//		int startIndex = objMesh.startIndex;
	//		m_nrTriangles = objMesh.triangleCount;
	//		m_nrVertices = objMesh.triangleCount * 3;

	//		for (int j = startIndex; j<startIndex + m_nrVertices; ++j)
	//		{
	//			uint idx = tempIdx[j];

	//			Vector3 v(vb[idx].position[0], vb[idx].position[1], vb[idx].position[2]);
	//			Vector3 n(vb[idx].normal[0], vb[idx].normal[1], vb[idx].normal[2]);
	//			Vector3 t(vb[idx].texCoord[0], vb[idx].texCoord[1], 0.0f);

	//			tmpVertices.push_back(v);
	//			tmpNormals.push_back(n);
	//			tmpTexCoords.push_back(t);
	//		}

	//		vec3 mi(math_maxfloat, math_maxfloat, math_maxfloat);
	//		vec3 ma(math_minfloat, math_minfloat, math_minfloat);

	//		for (uint i = 0; i<tmpVertices.size(); ++i)
	//		{
	//			vec3 v = tmpVertices[i];

	//			if (v.x > ma.x)
	//				ma.x = v.x;

	//			if (v.y > ma.y)
	//				ma.y = v.y;

	//			if (v.z > ma.z)
	//				ma.z = v.z;

	//			if (v.x < mi.x)
	//				mi.x = v.x;

	//			if (v.y < mi.y)
	//				mi.y = v.y;

	//			if (v.z < mi.z)
	//				mi.z = v.z;
	//		}

	//		vec3 center = (mi + ma) * 0.5;
	//		VertexBufferObjectAttribs::DATA *data = new VertexBufferObjectAttribs::DATA[tmpVertices.size()];

	//		for (uint i = 0; i<tmpVertices.size(); ++i)
	//		{
	//			vec3 v = tmpVertices[i];
	//			vec3 n = tmpNormals[i];
	//			vec3 t = tmpTexCoords[i];

	//			float tmpScale = 0.1f;
	//			vec3 tmpPos = (v - refCenter - center) * objScale + (center*objScale);

	//			v = tmpPos;

	//			data[i].vx = v.x;
	//			data[i].vy = v.y;
	//			data[i].vz = v.z;
	//			data[i].vw = 1.0f;

	//			data[i].cx = m_color.x;
	//			data[i].cy = m_color.y;
	//			data[i].cz = m_color.z;
	//			data[i].cw = m_color.w;

	//			data[i].nx = n.x;
	//			data[i].ny = n.y;
	//			data[i].nz = n.z;
	//			data[i].nw = 1.0f;

	//			data[i].tx = t.x;
	//			data[i].ty = t.y;
	//			data[i].tz = 0.0f;
	//			data[i].tw = 0.0f;
	//		}

	//		VertexBufferObjectAttribs* vboMesh = new VertexBufferObjectAttribs();
	//		vboMesh->setData(data, GL_STATIC_DRAW, tmpVertices.size(), GL_TRIANGLES);

	//		vboMesh->addAttrib(VERTEX_POSITION);
	//		vboMesh->addAttrib(VERTEX_NORMAL);
	//		vboMesh->addAttrib(VERTEX_COLOR);
	//		vboMesh->addAttrib(VERTEX_TEXTURE);
	//		vboMesh->bindAttribs();


	//		VertexBufferObjectAttribs* vboLines = new VertexBufferObjectAttribs();
	//		vboLines->setData(data, GL_STATIC_DRAW, tmpVertices.size(), GL_LINES);

	//		vboLines->addAttrib(VERTEX_POSITION);
	//		vboLines->addAttrib(VERTEX_NORMAL);
	//		vboLines->addAttrib(VERTEX_COLOR);
	//		vboLines->addAttrib(VERTEX_TEXTURE);
	//		vboLines->bindAttribs();

	//		delete[] data;

	//		vbosMesh.push_back(vboMesh);
	//		vbosLines.push_back(vboLines);
	//		matNames.push_back(objMesh.pMaterial->name);
	//	}

	//	delete model;

	//	m_vbosTrianglesAnimation.push_back(vbosMesh);
	//	m_vbosLinesAnimation.push_back(vbosLines);
	//	m_materialNamesAnimation.push_back(matNames);
	//}
}

void Object::buildVBOs(const std::string &fileName, const glm::vec3 &rot, const glm::vec3 &scale)
{
	ModelOBJ *model = new ModelOBJ();

	debugLog() << "OBSTACLEOBJ::load():" << fileName << std::endl;
	model->import(fileName.data());

	// read materials
	int nrMaterials = model->getNumberOfMaterials();

	for (int i = 0; i<nrMaterials; ++i)
	{
		std::experimental::filesystem::path file(fileName);
		std::string dir = file.parent_path().string();
		dir = std::regex_replace(dir, std::regex("\\\\"), "/");


		ModelOBJ::Material mat = model->getMaterial(i);
		std::string colorMapName = mat.colorMapFilename;
		colorMapName = std::regex_replace(colorMapName, std::regex("\\\\"), "/");

		ObjectMaterial info;
		info.texKd = new Texture(dir + "/" + colorMapName);

		m_materialCache.insert(std::make_pair(mat.name, info));
	}

	if (!m_materialCache.empty())
		m_hasMaterial = true;



	int nrMeshes = model->getNumberOfMeshes();

	const ModelOBJ::Vertex *vb = model->getVertexBuffer();
	const int *tempIdx = model->getIndexBuffer();

	//mat4 rotMatX = mat4::rotate(rot.x, 1.0f, 0.0f, 0.0f);
	//mat4 rotMatY = mat4::rotate(rot.y, 0.0f, 1.0f, 0.0f);
	//mat4 rotMatZ = mat4::rotate(rot.z, 0.0f, 0.0f, 1.0f);
	//mat4 scaleMat = mat4::scale(scale.x, scale.y, scale.z);

	for (int i = 0; i < nrMeshes; ++i)
	{
		std::vector<glm::vec3> tmpVertices;
		std::vector<glm::vec3> tmpNormals;
		std::vector<glm::vec3> tmpTexCoords;

		const ModelOBJ::Mesh &objMesh = model->getMesh(i);
		int startIndex = objMesh.startIndex;
		m_nrTriangles = objMesh.triangleCount;
		m_nrVertices = objMesh.triangleCount * 3;

		for (int j = startIndex; j<startIndex + m_nrVertices; ++j)
		{
			int idx = tempIdx[j];

			glm::vec3 v(vb[idx].position[0], vb[idx].position[1], vb[idx].position[2]);
			glm::vec3 n(vb[idx].normal[0], vb[idx].normal[1], vb[idx].normal[2]);
			glm::vec3 t(vb[idx].texCoord[0], vb[idx].texCoord[1], 0.0f);

			tmpVertices.push_back(v);
			tmpNormals.push_back(n);
			tmpTexCoords.push_back(t);
		}

		glm::vec3 mi(math_maxfloat, math_maxfloat, math_maxfloat);
		glm::vec3 ma(math_minfloat, math_minfloat, math_minfloat);

		for (int i = 0; i<tmpVertices.size(); ++i)
		{
			glm::vec3 v = tmpVertices[i];

			if (v.x > m_max.x)
				m_max.x = v.x;

			if (v.y > m_max.y)
				m_max.y = v.y;

			if (v.z > m_max.z)
				m_max.z = v.z;

			if (v.x < m_min.x)
				m_min.x = v.x;

			if (v.y < m_min.y)
				m_min.y = v.y;

			if (v.z < m_min.z)
				m_min.z = v.z;
		}

		m_center = (glm::vec4(m_min, 0.0f) + glm::vec4(m_max, 0.0f)) * 0.5f;

		VertexBufferObjectAttribs::DATA *data = new VertexBufferObjectAttribs::DATA[tmpVertices.size()];

		for (int i = 0; i<tmpVertices.size(); ++i)
		{
			glm::vec3 v = tmpVertices[i];
			glm::vec3 n = tmpNormals[i];
			glm::vec3 t = tmpTexCoords[i];

			data[i].vx = v.x;
			data[i].vy = v.y;
			data[i].vz = v.z;
			data[i].vw = 1.0f;

			data[i].cx = m_color.x;
			data[i].cy = m_color.y;
			data[i].cz = m_color.z;
			data[i].cw = m_color.w;

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

		vboMesh->addAttrib(VERTEX_POSITION);
		vboMesh->addAttrib(VERTEX_NORMAL);
		vboMesh->addAttrib(VERTEX_COLOR);
		vboMesh->addAttrib(VERTEX_TEXTURE);
		vboMesh->bindAttribs();


		VertexBufferObjectAttribs* vboLines = new VertexBufferObjectAttribs();
		vboLines->setData(data, GL_STATIC_DRAW, tmpVertices.size(), GL_LINES);

		vboLines->addAttrib(VERTEX_POSITION);
		vboLines->addAttrib(VERTEX_NORMAL);
		vboLines->addAttrib(VERTEX_COLOR);
		vboLines->addAttrib(VERTEX_TEXTURE);
		vboLines->bindAttribs();

		delete[] data;

		m_vbosTriangles.push_back(vboMesh);
		m_vbosLines.push_back(vboLines);
		m_materialNames.push_back(objMesh.pMaterial->name);
	}

	delete model;
}

void Object::updateAnimation(unsigned int fps)
{
	m_curAnimationTime += m_animationSpeed / fps;
	if (m_curAnimationTime > 1.0f)
	{
		int totalNum = m_vbosTrianglesAnimation.size();
		//loop(m_curAnimationIdx, 0, totalNum - 1, 1);
		m_curAnimationTime = 0.0f;
	}
}



//#include "Object.h"
//#include "Shader.h"
//#include "VertexBufferObjectAttribs.h"
//#include "ModelLoaderObj.h"
//#include "RenderContext.h"
//#include <iostream>
//
//Object::Object(const std::string &fileName, const glm::vec3 &pos, const glm::vec3 &scale, const glm::vec4 &rot, const glm::vec4 &color)
//: m_fileName(fileName),
//  m_position(pos),
//  m_scale(scale),
//  m_rotation(rot),
//  m_color(color),
//  m_shaderTriangles(NULL),
//  m_shaderLines(NULL),
//  m_shaderTrianglesDepth(NULL),
//  m_isSelected(false),
//  m_max(math_minfloat, math_minfloat, math_minfloat),
//  m_min(math_maxfloat, math_maxfloat, math_maxfloat),
//  m_nrTriangles(0),
//  m_nrVertices(0),
//  m_up(0, 1, 0, 1)
//{
//    init();
//}
//
//Object::~Object()
//{
//    delete m_shaderTriangles;
//    delete m_shaderLines;
//    delete m_shaderTrianglesDepth;
//
//    for(int i=m_vbosTriangles.size()-1; i>=0; --i)
//    {
//        VertexBufferObjectAttribs *vbo = m_vbosTriangles[i];
//        delete vbo;
//    }
//
//    for(int i=m_vbosLines.size()-1; i>=0; --i)
//    {
//        VertexBufferObjectAttribs *vbo = m_vbosLines[i];
//        delete vbo;
//    }
//}
//
//void Object::init()
//{
//    buildVBOs(m_fileName, glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
//
//    m_shaderTriangles = new Shader("../Shader/Object.vert.glsl", "../Shader/Object.frag.glsl");
//    m_shaderTriangles ->bindAttribLocations();
//
//    m_shaderTrianglesDepth = new Shader("../Shader/ObjectDepth.vert.glsl", "../Shader/ObjectDepth.frag.glsl");
//    m_shaderTrianglesDepth ->bindAttribLocations();
//
//    m_shaderLines = new Shader("../Shader/ObjectLines.vert.glsl", "../Shader/ObjectLines.frag.glsl");
//    m_shaderLines->bindAttribLocations();
//}
//
//void Object::render()
//{
//	auto param = RenderContext::globalObjectParam();
//	auto trans = RenderContext::transform();
//
//
//	glm::mat4 model = glm::translate(glm::mat4(1.0f), m_position);
//	model = glm::rotate(model, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
//	model = glm::scale(model, m_scale);
//	
//	glm::mat4 view = trans->view;
//	glm::mat4 projection = trans->projection;
//	glm::mat4 lightView = trans->lightView;
//
//
//    glEnable(GL_CLIP_DISTANCE0);    
//
//	m_shaderTriangles->bind();  
//
//        glActiveTexture(GL_TEXTURE1);
//        glBindTexture(GL_TEXTURE_2D, param->shadowMapID);    
//        m_shaderTriangles->seti("shadowMap", 1);   
//
//        glActiveTexture(GL_TEXTURE2);
//        glBindTexture(GL_TEXTURE_2D, param->shadowMapBlurredID);    
//        m_shaderTriangles->seti("shadowMapBlurred", 2);  
//
//	    m_shaderTriangles->setMatrix("matModel", model, GL_TRUE); 
//	    m_shaderTriangles->setMatrix("matView", view, GL_TRUE);
//	    m_shaderTriangles->setMatrix("matProjection", projection, GL_TRUE);
//        m_shaderTriangles->setMatrix("matLightView", lightView, GL_TRUE);
//
//        m_shaderTriangles->set3f("lightPos", param->lightPos);        
//        m_shaderTriangles->set3f("camPos", param->camPos);      
//        m_shaderTriangles->seti("applyShadow", param->applyShadow);
//        m_shaderTriangles->setf("shadowIntensity", param->shadowIntensity);
//        m_shaderTriangles->seti("isSelected", m_isSelected);
//
//        //m_shaderTrianglesDepth->set4f("clipPlane", param->clipPlaneGround);
//
//        for(int i=0; i<m_vbosTriangles.size(); ++i)
//        {
//            m_vbosTriangles[i]->render();
//        }
//
//	m_shaderTriangles->release();
//
//
//    if(param->renderMesh)
//    {
//	    m_shaderLines->bind();  
//
//	        m_shaderLines->setMatrix("matModel", model, GL_TRUE); 
//	        m_shaderLines->setMatrix("matView", view, GL_TRUE);
//	        m_shaderLines->setMatrix("matProjection", projection, GL_TRUE);
//
//            for(int i=0; i<m_vbosTriangles.size(); ++i)
//            {
//                m_vbosLines[i]->render();
//            }
//
//	    m_shaderLines->release();
//    }  
//}
//
//void Object::renderDepth()
//{
//	auto param = RenderContext::globalObjectParam();
//	auto trans = RenderContext::transform();
//
//    glEnable(GL_CLIP_DISTANCE0);
//
//	glm::mat4 model = glm::translate(glm::mat4(1.0f), m_position); // *mat4::rotateY(m_rotation.y) * mat4::scale(m_scale);
//	glm::mat4 view = trans->lightView;
//	glm::mat4 projection = trans->lightProjection;
//
//    //glCullFace(GL_FRONT);
//    //glFrontFace(GL_CCW);
//
//    //glClearDepth(1.0);
//    //glEnable(GL_POLYGON_OFFSET_FILL);
//    //glDepthFunc(GL_LEQUAL);
//    //glDepthRange(param.depthRangeMin, param.depthRangeMax);
//    //glPolygonOffset(param.polygonOffsetFactor, param.polygonOffsetUnits);
//    
//	m_shaderTrianglesDepth->bind();  
//
//	    m_shaderTrianglesDepth->setMatrix("matModel", model, GL_TRUE); 
//	    m_shaderTrianglesDepth->setMatrix("matView", view, GL_TRUE);
//	    m_shaderTrianglesDepth->setMatrix("matProjection", projection, GL_TRUE);     
//
//        //m_shaderTrianglesDepth->set4f("clipPlane", param->clipPlaneGround);
//
//        for(int i=0; i<m_vbosTriangles.size(); ++i)
//        {
//            m_vbosTriangles[i]->render();
//        }
//
//	m_shaderTrianglesDepth->release();
//}
//
//
//void Object::buildVBOs(const std::string &fileName, const glm::vec3 &rot, const glm::vec3 &scale)
//{
//    ModelOBJ *model = new ModelOBJ();
//
//	std::cout << "OBSTACLEOBJ::load():" << fileName << std::endl;
//    model->import(fileName.data());
//    
//	int nrMeshes = model->getNumberOfMeshes();
//
//    const ModelOBJ::Vertex *vb = model->getVertexBuffer();
//    const int *tempIdx = model->getIndexBuffer();
//
// //   mat4 rotMatX = mat4::rotate(rot.x, 1.0f, 0.0f, 0.0f);
//	//mat4 rotMatY = mat4::rotate(rot.y, 0.0f, 1.0f, 0.0f);
// //   mat4 rotMatZ = mat4::rotate(rot.z, 0.0f, 0.0f, 1.0f);
// //   mat4 scaleMat = mat4::scale(scale.x, scale.y, scale.z);
//
//	for(int i = 0; i < nrMeshes; ++i)	
//	{
//		std::vector<glm::vec3> tmpVertices;
//		std::vector<glm::vec3> tmpNormals;
//		std::vector<glm::vec3> tmpTexCoords;
//
//		const ModelOBJ::Mesh &objMesh = model->getMesh(i);
//		int startIndex = objMesh.startIndex;
//		m_nrTriangles = objMesh.triangleCount;
//		m_nrVertices = objMesh.triangleCount * 3;
//
//		for(int j=startIndex; j<startIndex + m_nrVertices; ++j)
//		{
//			int idx = tempIdx[j];
//
//			glm::vec3 v(vb[idx].position[0], vb[idx].position[1], vb[idx].position[2]);
//			glm::vec3 n(vb[idx].normal[0], vb[idx].normal[1], vb[idx].normal[2]);
//			glm::vec3 t(vb[idx].texCoord[0], vb[idx].texCoord[1], 0.0f);
//
//			tmpVertices.push_back(v);
//			tmpNormals.push_back(n);
//			tmpTexCoords.push_back(t);
//		} 
//
//        glm::vec3 mi(math_maxfloat, math_maxfloat, math_maxfloat);
//        glm::vec3 ma(math_minfloat, math_minfloat, math_minfloat);
//
//        for(int i=0; i<tmpVertices.size(); ++i)
//        {
//            glm::vec3 v = tmpVertices[i];
//            
//            if(v.x > m_max.x)
//                m_max.x = v.x;
//
//            if(v.y > m_max.y)
//                m_max.y = v.y;
//
//            if(v.z > m_max.z)
//                m_max.z = v.z;
//
//            if(v.x < m_min.x)
//                m_min.x = v.x;
//
//            if(v.y < m_min.y)
//                m_min.y = v.y;
//
//            if(v.z < m_min.z)
//                m_min.z = v.z;
//        }        
//
//        m_center =  (glm::vec4(m_min, 0.0f) + glm::vec4(m_max, 0.0f)) * 0.5f;
//
//        VertexBufferObjectAttribs::DATA *data = new VertexBufferObjectAttribs::DATA[tmpVertices.size()];
//
//		for(int i=0; i<tmpVertices.size(); ++i)
//		{
//            glm::vec3 v = tmpVertices[i];
//            glm::vec3 n = tmpNormals[i];
//            glm::vec3 t = tmpTexCoords[i];
//
//			data[i].vx = v.x;
//			data[i].vy = v.y;
//			data[i].vz = v.z;
//			data[i].vw = 1.0f;
//
//			data[i].cx = m_color.x;
//			data[i].cy = m_color.y;
//			data[i].cz = m_color.z;
//			data[i].cw = m_color.w;
//			
//            data[i].nx = n.x;
//			data[i].ny = n.y;
//			data[i].nz = n.z;
//			data[i].nw = 1.0f;
//            
//			data[i].tx = t.x;
//			data[i].ty = t.y;
//            data[i].tz = 0.0f;
//            data[i].tw = 0.0f;
//		}
//
//		VertexBufferObjectAttribs* vboMesh = new VertexBufferObjectAttribs();
//		vboMesh->setData(data, GL_STATIC_DRAW, tmpVertices.size(), GL_TRIANGLES); 
//
//		vboMesh->addAttrib(VERTEX_POSITION);
//		vboMesh->addAttrib(VERTEX_NORMAL);
//		vboMesh->addAttrib(VERTEX_COLOR);
//		vboMesh->addAttrib(VERTEX_TEXTURE);
//		vboMesh->bindAttribs();
//
//
//		VertexBufferObjectAttribs* vboLines = new VertexBufferObjectAttribs();
//		vboLines->setData(data, GL_STATIC_DRAW, tmpVertices.size(), GL_LINES); 
//
//		vboLines->addAttrib(VERTEX_POSITION);
//		vboLines->addAttrib(VERTEX_NORMAL);
//		vboLines->addAttrib(VERTEX_COLOR);
//		vboLines->addAttrib(VERTEX_TEXTURE);
//		vboLines->bindAttribs();
//
//		delete[] data;
//
//        m_vbosTriangles.push_back(vboMesh);
//        m_vbosLines.push_back(vboLines);
//    }
//}
