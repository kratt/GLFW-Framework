#include "Object.h"
#include "Shader.h"
#include "Common.h"
#include "VertexBufferObject.h"
#include "ModelLoaderObj.h"
#include "Texture.h"
#include "RenderContext.h"
#include "log\Log.h"
#include "log\vec.h"
#include "system_utils.h"

#include <regex> 
#include <algorithm> 

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
		delete m_vbosTriangles[i];
	}

	for (int i = m_vbosLines.size() - 1; i >= 0; --i)
	{
		delete m_vbosLines[i];
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
		std::vector<VertexBufferObject*> &curVBO = m_vbosTrianglesAnimation[m_curAnimationIdx];
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


void Object::buildVBOsAnimation(const std::string &path, const glm::vec3 &rot, const glm::vec3 &scale)
{
	std::vector<std::string> fileList = utils::get_file_list(path, ".obj");

	// first determine scaling factor based on first obj and materials
	float objScale = 1.0f;
	glm::vec3 refCenter = glm::vec3(0.0f);

	if (!fileList.empty())
	{
		ModelOBJ *model = new ModelOBJ();
		model->import(fileList[0].data());

		int nrMaterials = model->getNumberOfMaterials();

		for (int i = 0; i<nrMaterials; ++i)
		{
			std::string dir = std::regex_replace(path, std::regex("\\\\"), "/");

			ModelOBJ::Material mat = model->getMaterial(i);
			std::string colorMapName = mat.colorMapFilename;
			colorMapName = std::regex_replace(colorMapName, std::regex("\\\\"), "/");

			ObjectMaterial info;
			info.texKd = new Texture(dir + "/" + colorMapName);

			debugLog() << dir + "/" + colorMapName << std::endl;
			m_materialCache.insert(std::make_pair(mat.name, info));
		}

		if (!m_materialCache.empty())
			m_hasMaterial = true;

		int nrMeshes = model->getNumberOfMeshes();
		const ModelOBJ::Vertex *vb = model->getVertexBuffer();
		const int *tempIdx = model->getIndexBuffer();

		glm::vec3 mi(math_maxfloat, math_maxfloat, math_maxfloat);
		glm::vec3 ma(math_minfloat, math_minfloat, math_minfloat);

		for (int i = 0; i < nrMeshes; ++i)
		{
			std::vector<glm::vec3> tmpVertices;

			const ModelOBJ::Mesh &objMesh = model->getMesh(i);

			int startIndex = objMesh.startIndex;
			int nrVertices = objMesh.triangleCount * 3;

			for (int j = startIndex; j<startIndex + nrVertices; ++j)
			{
				int idx = tempIdx[j];
				glm::vec3 v = glm::vec3(vb[idx].position[0], vb[idx].position[1], vb[idx].position[2]);

				ma.x = std::max(v.x, ma.x);
				ma.y = std::max(v.y, ma.y);
				ma.z = std::max(v.z, ma.z);

				mi.x = std::min(v.x, mi.x);
				mi.y = std::min(v.y, mi.y);
				mi.z = std::min(v.z, mi.z);
			}
		}

		glm::vec3 dist = ma - mi;

		if (dist.x > dist.y && dist.x > dist.z) objScale = 1.0f / dist.x;
		if (dist.y > dist.x && dist.y > dist.z) objScale = 1.0f / dist.y;
		if (dist.z > dist.x && dist.z > dist.y) objScale = 1.0f / dist.z;

		objScale *= 1.0f; // optional
		refCenter = (mi + ma) * 0.5f;

		delete model;
	}

	for (int i = 0; i < fileList.size(); ++i)
	{
		std::vector<VertexBufferObject*> vbosMesh;
		std::vector<std::string> matNames;

		ModelOBJ *model = new ModelOBJ();

		debugLog() << "load():" << fileList[i] << std::endl;
		model->import(fileList[i].data());


		// collect data for each material. Put vertices into the same vbo if they share the same material
		std::map<std::string, std::vector<VertexData>> v_data;

		int nrMeshes = model->getNumberOfMeshes();
		const ModelOBJ::Vertex *vb = model->getVertexBuffer();
		const int *tempIdx = model->getIndexBuffer();

		for (int i = 0; i < nrMeshes; ++i)
		{
			const ModelOBJ::Mesh &objMesh = model->getMesh(i);
			auto &data = v_data[objMesh.pMaterial->name];

			int startIndex = objMesh.startIndex;
			m_nrTriangles = objMesh.triangleCount;
			m_nrVertices = objMesh.triangleCount * 3;

			glm::vec3 mi(math_maxfloat, math_maxfloat, math_maxfloat);
			glm::vec3 ma(math_minfloat, math_minfloat, math_minfloat);

			int numVerts = 0;
			for (int j = startIndex; j < startIndex + m_nrVertices; ++j)
			{
				int idx = tempIdx[j];

				glm::vec3 pos = glm::vec3(vb[idx].position[0], vb[idx].position[1], vb[idx].position[2]);
				glm::vec3 normal = glm::vec3(vb[idx].normal[0], vb[idx].normal[1], vb[idx].normal[2]);
				glm::vec3 texCoords = glm::vec3(vb[idx].texCoord[0], vb[idx].texCoord[1], 0.0f);

				ma.x = std::max(pos.x, ma.x);
				ma.y = std::max(pos.y, ma.y);
				ma.z = std::max(pos.z, ma.z);

				mi.x = std::min(pos.x, mi.x);
				mi.y = std::min(pos.y, mi.y);
				mi.z = std::min(pos.z, mi.z);

				data.push_back(VertexData(pos, normal, glm::vec3(0.0f), texCoords));

				++numVerts;
			}

			glm::vec3 center = (mi + ma) * 0.5f;

			auto start = data.end() - numVerts;
			auto end = data.end();
			std::transform(start, end, start, [&](VertexData &d)
			{
				glm::vec3 pos = glm::vec3(d.vx, d.vy, d.vz);
				pos = (pos - refCenter - center) * objScale + (center*objScale);
				d.vx = pos.x;
				d.vy = pos.y;
				d.vz = pos.z;

				return d;
			});
		}

		for (auto &data_pair : v_data)
		{
			VertexBufferObject* vboMesh = new VertexBufferObject();

			vboMesh->setData(data_pair.second, GL_STATIC_DRAW, data_pair.second.size(), GL_TRIANGLES);

			vboMesh->addAttrib(VERTEX_POSITION);
			vboMesh->addAttrib(VERTEX_NORMAL);
			vboMesh->addAttrib(VERTEX_COLOR);
			vboMesh->addAttrib(VERTEX_TEXTURE);
			vboMesh->bindAttribs();

			vbosMesh.push_back(vboMesh);
			matNames.push_back(data_pair.first);
		}

		delete model;

		m_vbosTrianglesAnimation.push_back(vbosMesh);
		m_materialNamesAnimation.push_back(matNames);
	}
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


	// collect data for each material. Put vertices into the same vbo if they share the same material
	std::map<std::string, std::vector<VertexData>> v_data;

	int nrMeshes = model->getNumberOfMeshes();

	const ModelOBJ::Vertex *vb = model->getVertexBuffer();
	const int *tempIdx = model->getIndexBuffer();

	for (int i = 0; i < nrMeshes; ++i)
	{
		std::vector<glm::vec3> tmpVertices;
		std::vector<glm::vec3> tmpNormals;
		std::vector<glm::vec3> tmpTexCoords;

		const ModelOBJ::Mesh &objMesh = model->getMesh(i);

		auto &data = v_data[objMesh.pMaterial->name];

		int startIndex = objMesh.startIndex;
		m_nrTriangles = objMesh.triangleCount;
		m_nrVertices = objMesh.triangleCount * 3;

		for (int j = startIndex; j < startIndex + m_nrVertices; ++j)
		{
			int idx = tempIdx[j];

			data.push_back(VertexData(glm::vec3(vb[idx].position[0], vb[idx].position[1], vb[idx].position[2]),
				glm::vec3(vb[idx].normal[0], vb[idx].normal[1], vb[idx].normal[2]), glm::vec3(0.0f),
				glm::vec3(vb[idx].texCoord[0], vb[idx].texCoord[1], 0.0f)));
		}
	}

	for (auto &data_pair : v_data)
	{
		VertexBufferObject* vboMesh = new VertexBufferObject();

		vboMesh->setData(data_pair.second, GL_STATIC_DRAW, data_pair.second.size(), GL_TRIANGLES);

		vboMesh->addAttrib(VERTEX_POSITION);
		vboMesh->addAttrib(VERTEX_NORMAL);
		vboMesh->addAttrib(VERTEX_COLOR);
		vboMesh->addAttrib(VERTEX_TEXTURE);
		vboMesh->bindAttribs();

		m_vbosTriangles.push_back(vboMesh);
		m_materialNames.push_back(data_pair.first);
	}

	delete model;
}

void Object::updateAnimation(unsigned int fps)
{
	m_curAnimationTime += m_animationSpeed / fps;
	if (m_curAnimationTime > 1.0f)
	{
		int totalNum = m_vbosTrianglesAnimation.size();
		Common::loop(m_curAnimationIdx, 0, totalNum - 1, 1);
		m_curAnimationTime = 0.0f;
	}
}