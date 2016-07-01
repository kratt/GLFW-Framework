// Computer Graphics and Media Informatics
// University of Konstanz
// 2015

#include "Shader.h"

#include <iostream>
#include <cstdio>
#include <agents.h>
#include <functional>
#include "CallbackTimer.h"


Shader::Shader()
: m_id(0),
  m_vertProg(0),
  m_contProg(0),
  m_evalProg(0),
  m_geomProg(0),
  m_fragProg(0),
  m_vFileName(nullptr),
  m_cFileName(nullptr),
  m_eFileName(nullptr),
  m_gFileName(nullptr),
  m_fFileName(nullptr),
  m_firstUpdate(true),
  m_vertIsDirty(false),
  m_fragIsDirty(false)
{  
	if (!m_id) {
		m_id = glCreateProgram();
	}
}

Shader::Shader(const GLchar *vFileName, const GLchar *fFileName, bool autoUpdate)
: m_id(0),
  m_vertProg(0),
  m_contProg(0),
  m_evalProg(0),
  m_geomProg(0),
  m_fragProg(0),
  m_vFileName(nullptr),
  m_cFileName(nullptr),
  m_eFileName(nullptr),
  m_gFileName(nullptr),
  m_fFileName(nullptr),
  m_firstUpdate(true),
  m_vertIsDirty(false),   
  m_fragIsDirty(false),
  m_geomIsDirty(false),
  m_contIsDirty(false),
  m_evalIsDirty(false),
  m_autoUpdate(autoUpdate)
{  
	if (!m_id) {
		m_id = glCreateProgram();
	}

	attachVertexShader(vFileName);
	attachFragmentShader(fFileName);
	bindAttribLocations();
	
	if(m_autoUpdate)
		initTimer();
}

Shader::Shader(const GLchar *vFileName, const GLchar *gFileName, const GLchar *fFileName, bool autoUpdate)
: m_id(0),
  m_vertProg(0),
  m_contProg(0),
  m_evalProg(0),
  m_geomProg(0),
  m_fragProg(0),
  m_vFileName(nullptr),
  m_cFileName(nullptr),
  m_eFileName(nullptr),
  m_gFileName(nullptr),
  m_fFileName(nullptr),
  m_firstUpdate(true),
  m_vertIsDirty(false),
  m_fragIsDirty(false),
  m_geomIsDirty(false),
  m_contIsDirty(false),
  m_evalIsDirty(false),
 m_autoUpdate(autoUpdate)
{
	if (!m_id) {
		m_id = glCreateProgram();
	}

	attachVertexShader(vFileName);
	attachGeometryShader(gFileName);
	attachFragmentShader(fFileName);
	bindAttribLocations();

	if (m_autoUpdate)
		initTimer();
}

Shader::~Shader()
{
	cleanUp();
}

void Shader::initTimer()
{
	start_callback_timer(std::bind(&Shader::autoUpdate, this), 1000);
}

void Shader::attachVertexShader(const GLchar *fileName)
{
	if(fileName)
	{
		m_vFileName = fileName;
		const GLchar *source = readFile(fileName);

		if(source)
		{
			m_vertProg = compile(source, GL_VERTEX_SHADER);
			glAttachShader(m_id, m_vertProg);

			glLinkProgram(m_id);

			printf("SHADER::attachVertexShader():attached: %s\n", fileName);
		}
		else
		{
			printf("SHADER::attachVertexShader():not found: %s\n", fileName);
			std::cin.get();
		}
	}
}

void Shader::attachControlShader(const GLchar *fileName)
{
	if(fileName)
	{
		m_cFileName = fileName;
		const GLchar *source = readFile(fileName);

		if(source)
		{
			m_contProg = compile(source, GL_TESS_CONTROL_SHADER);
			glAttachShader(m_id, m_contProg);

			glLinkProgram(m_id);

			printf("SHADER::attachControlShader():attached: %s\n", fileName);
		}
		else
		{
			printf("SHADER::attachControlShader():not found: %s\n", fileName);
			std::cin.get();
		}

	}
}

void Shader::attachEvaluationShader(const GLchar *fileName)
{
	if(fileName)
	{
		m_eFileName = fileName;
		const GLchar *source = readFile(fileName);

		if(source)
		{
			m_evalProg = compile(source, GL_TESS_EVALUATION_SHADER);
			glAttachShader(m_id, m_evalProg);

			glLinkProgram(m_id);

			printf("SHADER::attachEvaluationShader():attached: %s\n", fileName);
		}
		else
		{
			printf("SHADER::attachEvaluationShader():not found: %s\n", fileName);
			std::cin.get();
		}
	}
}

void Shader::attachGeometryShader(const GLchar *fileName)
{
	if(fileName)
	{
		m_gFileName = fileName;
		const GLchar *source = readFile(fileName);

		if(source)
		{
			m_geomProg = compile(source, GL_GEOMETRY_SHADER);
			glAttachShader(m_id, m_geomProg);

			glLinkProgram(m_id);

			printf("SHADER::attachGeometryShader():attached: %s\n", fileName);
		}
		else
		{
			printf("SHADER::attachGeometryShader():not found: %s\n", fileName);
			std::cin.get();
		}
	}
}

void Shader::attachFragmentShader(const GLchar *fileName)
{
	if(fileName)
	{
		m_fFileName = fileName;
		const GLchar *source = readFile(fileName);

		if(source)
		{
			m_fragProg = compile(source, GL_FRAGMENT_SHADER);
			glAttachShader(m_id, m_fragProg);

			glLinkProgram(m_id);

			printf("SHADER::attachFragmentShader():attached: %s\n", fileName);
		}
		else
		{
			printf("SHADER::attachFragmentShader():not found: %s\n", fileName);
			std::cin.get();
		}
	}
}

void Shader::autoUpdate()
{
	checkFile(m_vFileName, m_vOldDateTime, GL_VERTEX_SHADER);
	checkFile(m_cFileName, m_cOldDateTime, GL_TESS_CONTROL_SHADER);
	checkFile(m_eFileName, m_eOldDateTime, GL_TESS_EVALUATION_SHADER);
	checkFile(m_gFileName, m_gOldDateTime, GL_GEOMETRY_SHADER);
	checkFile(m_fFileName, m_fOldDateTime, GL_FRAGMENT_SHADER);

	m_firstUpdate = false;
}

void Shader::checkFile(const char *fileName, std::experimental::filesystem::file_time_type &oldTime, GLuint type)
{
	if (!fileName)
		return;

	std::experimental::filesystem::file_time_type ftime = std::experimental::filesystem::last_write_time(fileName);

	if (ftime != oldTime)
	{
		if (!m_firstUpdate)
		{
			if (type == GL_VERTEX_SHADER)           m_vertIsDirty = true;	
			if (type == GL_TESS_CONTROL_SHADER)     m_contIsDirty = true;
			if (type == GL_TESS_EVALUATION_SHADER)  m_evalIsDirty = true;
			if (type == GL_GEOMETRY_SHADER)         m_geomIsDirty = true;
			if (type == GL_FRAGMENT_SHADER)         m_fragIsDirty = true;
		}

		oldTime = ftime;
	}
}

void Shader::reload()
{
	if (m_vertIsDirty)
	{
		glDetachShader(m_id, m_vertProg);
		glDeleteShader(m_vertProg);

		attachVertexShader(m_vFileName);
		glLinkProgram(m_id);
		std::cout << "SHADER::Vertex updated" << std::endl;

		m_vertIsDirty = false;
	}

	if (m_contIsDirty)
	{
		glDetachShader(m_id, m_contProg);
		glDeleteShader(m_contProg);

		attachControlShader(m_cFileName);
		glLinkProgram(m_id);
		std::cout << "SHADER::Control updated" << std::endl;

		m_contIsDirty = false;
	}


	if (m_evalIsDirty)
	{
		glDetachShader(m_id, m_evalProg);
		glDeleteShader(m_evalProg);

		attachEvaluationShader(m_eFileName);
		glLinkProgram(m_id);
		std::cout << "SHADER::Evaluation updated" << std::endl;;

		m_evalIsDirty = false;
	}

	if (m_geomIsDirty)
	{
		glDetachShader(m_id, m_geomProg);
		glDeleteShader(m_geomProg);

		attachGeometryShader(m_gFileName);

		glLinkProgram(m_id);
		std::cout << "SHADER::Geometry updated" << std::endl;

		m_geomIsDirty = false;
	}

	if (m_fragIsDirty)
	{
		glDetachShader(m_id, m_fragProg);
		glDeleteShader(m_fragProg);

		attachFragmentShader(m_fFileName);

		glLinkProgram(m_id);
		std::cout << "SHADER::Fragment updated" << std::endl;

		m_fragIsDirty = false;
	}	
}

const GLchar *Shader::readFile(const GLchar *fileName)
{
	FILE *fp = NULL;
	GLchar *content = NULL;
	GLint count = 0;

	if (fileName) 
	{
		fp = fopen(fileName, "rt");

		if (fp) 
		{      
			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);

			if (count > 0) 
			{
				content = new GLchar[count + 1];
				count = fread(content, sizeof(GLchar), count, fp);
				content[count] = '\0';
			}

			fclose(fp);
		}
	}

	return content;
}

GLuint Shader::compile(const GLchar *source, GLuint type)
{
	GLuint id = glCreateShader(type);
	glShaderSource(id, 1, &source, NULL);
	glCompileShader(id);
	
	GLint c = 0;
	
	glGetShaderiv(id, GL_COMPILE_STATUS, &c);

	if(!c)
	{
		GLchar* logstr = new GLchar[2048];
		glGetShaderInfoLog(id, 2048, NULL, logstr);
		printf("SHADER::Error compiling shader %s:\n%s\n", source, logstr);
		delete[] logstr;
		return 0;
	}

	return id;
}

void Shader::cleanUp()
{
	if(m_id)
	{
		if(m_vertProg)
		{
			glDetachShader(m_id, m_vertProg);
			glDeleteShader(m_vertProg);
			m_vertProg = 0;
		}

		if(m_contProg)
		{
			glDetachShader(m_id, m_contProg);
			glDeleteShader(m_contProg);
			m_contProg = 0;
		}

		if(m_evalProg)
		{
			glDetachShader(m_id, m_evalProg);
			glDeleteShader(m_evalProg);
			m_evalProg = 0;
		}

		if(m_geomProg)
		{
			glDetachShader(m_id, m_geomProg);
			glDeleteShader(m_geomProg);
			m_geomProg = 0;
		}

		if(m_fragProg)
		{
			glDetachShader(m_id, m_fragProg);
			glDeleteShader(m_fragProg);
			m_fragProg = 0;
		}

		glDeleteProgram(m_id);
		m_id = 0;
	}
}

GLuint Shader::id() const
{
	return m_id;
}

void Shader::bind()
{
	if(m_autoUpdate)
		reload();

	glUseProgram(m_id);
}

void Shader::release() const
{
	glUseProgram(0);
}

void Shader::link() const
{
	glLinkProgram(m_id);
}

void Shader::bindAttribLocation(const GLchar *label, GLuint attribID)
{    
	glBindAttribLocation(m_id, attribID, label);
}

void Shader::testAutoUpdate()
{
	std::cout << "Shader update" << std::endl;
}

void Shader::seti(const GLchar* label, GLint arg)
{
	glUniform1i(glGetUniformLocation(m_id, label), arg);
}

void Shader::setf(const GLchar* label, GLfloat arg)
{
	glUniform1f(glGetUniformLocation(m_id, label), arg);
}

void Shader::set2i(const GLchar* label, GLint arg1, GLint arg2)
{
	glUniform2i(glGetUniformLocation(m_id, label), arg1, arg2);
}

void Shader::set2f(const GLchar* label, GLfloat arg1, GLfloat arg2)
{
	glUniform2f(glGetUniformLocation(m_id, label), arg1, arg2);
}

void Shader::set2f(const GLchar* label, const glm::vec2 &v)
{
	glUniform2f(glGetUniformLocation(m_id, label), v.x, v.y);
}

void Shader::set3i(const GLchar* label, GLint arg1, GLint arg2, GLint arg3)
{
	glUniform3i(glGetUniformLocation(m_id, label), arg1, arg2, arg3);
}

void Shader::set3f(const GLchar* label, GLfloat arg1, GLfloat arg2, GLfloat arg3)
{
	glUniform3f(glGetUniformLocation(m_id, label), arg1, arg2, arg3);
}

void Shader::set3f(const GLchar* label, const glm::vec3 &v)
{
	glUniform3f(glGetUniformLocation(m_id, label), v.x, v.y, v.z);
}

void Shader::set4i(const GLchar* label, GLint arg1, GLint arg2, GLint arg3, GLint arg4)
{
	glUniform4i(glGetUniformLocation(m_id, label), arg1, arg2, arg3, arg4);
}

void Shader::set4f(const GLchar * label, const glm::vec4 & v)
{
	glUniform4f(glGetUniformLocation(m_id, label), v.x, v.y, v.z, v.w);
}

void Shader::set4f(const GLchar* label, GLfloat arg1, GLfloat arg2, GLfloat arg3, GLfloat arg4)
{
	glUniform4f(glGetUniformLocation(m_id, label), arg1, arg2, arg3, arg4);
}

void Shader::set3iv(const GLchar* label, const GLint* args)
{
	glUniform3iv(glGetUniformLocation(m_id, label), 1, args);
}

void Shader::set3fv(const GLchar* label, const GLfloat* args)
{
	glUniform3fv(glGetUniformLocation(m_id, label), 1, args);
}

void Shader::setMatrix(const GLchar* label, const GLfloat* m, GLboolean transpose)
{
	glUniformMatrix4fv(glGetUniformLocation(m_id, label), 1, transpose, m);
}

void Shader::setMatrix(const GLchar* label, const GLdouble* m, GLboolean transpose)
{
	GLfloat fm[] = {(GLfloat)m[0], (GLfloat)m[1], (GLfloat)m[2], (GLfloat)m[3], (GLfloat)m[4], (GLfloat)m[5], (GLfloat)m[6], (GLfloat)m[7], (GLfloat)m[8], (GLfloat)m[9], (GLfloat)m[10], (GLfloat)m[11], (GLfloat)m[12], (GLfloat)m[13], (GLfloat)m[14], (GLfloat)m[15]};
	glUniformMatrix4fv(glGetUniformLocation(m_id, label), 1, transpose, fm);
}

void Shader::setMatrix(const GLchar* label, const glm::mat4x4 &mat, GLboolean transpose)
{
	glUniformMatrix4fv(glGetUniformLocation(m_id, label), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMatrix(const GLchar* label, const glm::mat3x3 &mat, GLboolean transpose)
{
	glUniformMatrix4fv(glGetUniformLocation(m_id, label), 1, GL_FALSE, &mat[0][0]);
}

void Shader::bindAttribLocations()
{
	this->bindAttribLocation("Position", VERTEX_POSITION);
	this->bindAttribLocation("Normal", VERTEX_NORMAL);
	this->bindAttribLocation("Color", VERTEX_COLOR);
	this->bindAttribLocation("Texture", VERTEX_TEXTURE);
}