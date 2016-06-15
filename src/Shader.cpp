// Computer Graphics and Media Informatics
// University of Konstanz
// 2015

#include "Shader.h"

#include <iostream>
#include <cstdio>


Shader::Shader()
: m_id(0),
  m_vertProg(0),
  m_contProg(0),
  m_evalProg(0),
  m_geomProg(0),
  m_fragProg(0)
{  
	if (!m_id) {
		m_id = GL_ASSERT( glCreateProgram() );
	}
}

Shader::Shader(const GLchar *vFileName, const GLchar *fFileName)
: m_id(0),
  m_vertProg(0),
  m_contProg(0),
  m_evalProg(0),
  m_geomProg(0),
  m_fragProg(0)
{  
	if (!m_id) {
		m_id = GL_ASSERT( glCreateProgram() );
	}

	attachVertexShader(vFileName);
	attachFragmentShader(fFileName);
	bindAttribLocations();
}

Shader::Shader(const GLchar *vFileName, const GLchar *gFileName, const GLchar *fFileName)
: m_id(0),
  m_vertProg(0),
  m_contProg(0),
  m_evalProg(0),
  m_geomProg(0),
  m_fragProg(0)
{
	if (!m_id) {
		m_id = GL_ASSERT( glCreateProgram() );
	}


	attachVertexShader(vFileName);
	attachGeometryShader(gFileName);
	attachFragmentShader(fFileName);
	bindAttribLocations();
}

Shader::~Shader()
{
	cleanUp();
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
			GL_ASSERT( glAttachShader(m_id, m_vertProg) );

			GL_ASSERT( glLinkProgram(m_id) );

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
			GL_ASSERT( glAttachShader(m_id, m_contProg) );

			GL_ASSERT( glLinkProgram(m_id) );

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
			GL_ASSERT( glAttachShader(m_id, m_evalProg) );

			GL_ASSERT( glLinkProgram(m_id) );

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
			GL_ASSERT( glAttachShader(m_id, m_geomProg) );

			GL_ASSERT( glLinkProgram(m_id) );

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
			GL_ASSERT( glAttachShader(m_id, m_fragProg) );

			GL_ASSERT( glLinkProgram(m_id) );

			printf("SHADER::attachFragmentShader():attached: %s\n", fileName);
		}
		else
		{
			printf("SHADER::attachFragmentShader():not found: %s\n", fileName);
			std::cin.get();
		}
	}
}

void Shader::checkFile(const GLchar *fileName, GLuint type)
{
	if(type == GL_VERTEX_SHADER)
	{
		GL_ASSERT( glDetachShader(m_id, m_vertProg) );
		GL_ASSERT( glDeleteShader(m_vertProg) );

		attachVertexShader(fileName);
		printf("SHADER::Vertex updated");
	}

	if(type == GL_TESS_CONTROL_SHADER)
	{
		GL_ASSERT( glDetachShader(m_id, m_contProg) );
		GL_ASSERT( glDeleteShader(m_contProg) );

		attachControlShader(fileName);
		printf("SHADER::Control updated");
	}

	if(type == GL_TESS_EVALUATION_SHADER)
	{
		GL_ASSERT( glDetachShader(m_id, m_evalProg) );
		GL_ASSERT( glDeleteShader(m_evalProg) );

		attachEvaluationShader(fileName);
		printf("SHADER::Evaluation updated");
	}

	if(type == GL_GEOMETRY_SHADER)
	{
		GL_ASSERT( glDetachShader(m_id, m_geomProg) );
		GL_ASSERT( glDeleteShader(m_geomProg) );

		attachGeometryShader(fileName);
		printf("SHADER::Geometry updated");
	}

	if(type == GL_FRAGMENT_SHADER)
	{
		GL_ASSERT( glDetachShader(m_id, m_fragProg) );
		GL_ASSERT( glDeleteShader(m_fragProg) );

		attachFragmentShader(fileName);
		printf("SHADER::Fragment updated");
	}

	GL_ASSERT( glLinkProgram(m_id) );
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
	GLuint id = GL_ASSERT( glCreateShader(type) );
	GL_ASSERT( glShaderSource(id, 1, &source, NULL) );
	GL_ASSERT( glCompileShader(id) );
	
	GLint c = 0;
	
	GL_ASSERT( glGetShaderiv(id, GL_COMPILE_STATUS, &c) );

	if(!c)
	{
		GLchar* logstr = new GLchar[2048];
		GL_ASSERT( glGetShaderInfoLog(id, 2048, NULL, logstr) );
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
			GL_ASSERT( glDetachShader(m_id, m_vertProg) );
			GL_ASSERT( glDeleteShader(m_vertProg) );
			m_vertProg = 0;
		}

		if(m_contProg)
		{
			GL_ASSERT( glDetachShader(m_id, m_contProg) );
			GL_ASSERT( glDeleteShader(m_contProg) );
			m_contProg = 0;
		}

		if(m_evalProg)
		{
			GL_ASSERT( glDetachShader(m_id, m_evalProg) );
			GL_ASSERT( glDeleteShader(m_evalProg) );
			m_evalProg = 0;
		}

		if(m_geomProg)
		{
			GL_ASSERT( glDetachShader(m_id, m_geomProg) );
			GL_ASSERT( glDeleteShader(m_geomProg) );
			m_geomProg = 0;
		}

		if(m_fragProg)
		{
			GL_ASSERT( glDetachShader(m_id, m_fragProg) );
			GL_ASSERT( glDeleteShader(m_fragProg) );
			m_fragProg = 0;
		}

		GL_ASSERT( glDeleteProgram(m_id) );
		m_id = 0;
	}
}

GLuint Shader::id() const
{
	return m_id;
}

void Shader::bind() const
{
	GL_ASSERT( glUseProgram(m_id) );
}

void Shader::release() const
{
	GL_ASSERT( glUseProgram(0) );
}

void Shader::link() const
{
	GL_ASSERT( glLinkProgram(m_id) );
}

void Shader::bindAttribLocation(const GLchar *label, GLuint attribID)
{    
	GL_ASSERT( glBindAttribLocation(m_id, attribID, label) );
}

void Shader::seti(const GLchar* label, GLint arg)
{
	GL_ASSERT( glUniform1i(glGetUniformLocation(m_id, label), arg) );
}

void Shader::setf(const GLchar* label, GLfloat arg)
{
	GL_ASSERT( glUniform1f(glGetUniformLocation(m_id, label), arg) );
}

void Shader::set2i(const GLchar* label, GLint arg1, GLint arg2)
{
	GL_ASSERT( glUniform2i(glGetUniformLocation(m_id, label), arg1, arg2) );
}

void Shader::set2f(const GLchar* label, GLfloat arg1, GLfloat arg2)
{
	GL_ASSERT( glUniform2f(glGetUniformLocation(m_id, label), arg1, arg2) );
}

void Shader::set3i(const GLchar* label, GLint arg1, GLint arg2, GLint arg3)
{
	GL_ASSERT( glUniform3i(glGetUniformLocation(m_id, label), arg1, arg2, arg3) );
}

void Shader::set3f(const GLchar* label, GLfloat arg1, GLfloat arg2, GLfloat arg3)
{
	GL_ASSERT( glUniform3f(glGetUniformLocation(m_id, label), arg1, arg2, arg3) );
}

void Shader::set3f(const GLchar* label, const glm::vec3 &v)
{
	GL_ASSERT( glUniform3f(glGetUniformLocation(m_id, label), v.x, v.y, v.z) );
}

void Shader::set4i(const GLchar* label, GLint arg1, GLint arg2, GLint arg3, GLint arg4)
{
	GL_ASSERT( glUniform4i(glGetUniformLocation(m_id, label), arg1, arg2, arg3, arg4) );
}

void Shader::set4f(const GLchar* label, GLfloat arg1, GLfloat arg2, GLfloat arg3, GLfloat arg4)
{
	GL_ASSERT( glUniform4f(glGetUniformLocation(m_id, label), arg1, arg2, arg3, arg4) );
}

void Shader::set3iv(const GLchar* label, const GLint* args)
{
	GL_ASSERT( glUniform3iv(glGetUniformLocation(m_id, label), 1, args) );
}

void Shader::set3fv(const GLchar* label, const GLfloat* args)
{
	GL_ASSERT( glUniform3fv(glGetUniformLocation(m_id, label), 1, args) );
}

void Shader::setMatrix(const GLchar* label, const GLfloat* m, GLboolean transpose)
{
	GL_ASSERT( glUniformMatrix4fv(glGetUniformLocation(m_id, label), 1, transpose, m) );
}

void Shader::setMatrix(const GLchar* label, const GLdouble* m, GLboolean transpose)
{
	GLfloat fm[] = {(GLfloat)m[0], (GLfloat)m[1], (GLfloat)m[2], (GLfloat)m[3], (GLfloat)m[4], (GLfloat)m[5], (GLfloat)m[6], (GLfloat)m[7], (GLfloat)m[8], (GLfloat)m[9], (GLfloat)m[10], (GLfloat)m[11], (GLfloat)m[12], (GLfloat)m[13], (GLfloat)m[14], (GLfloat)m[15]};
	GL_ASSERT( glUniformMatrix4fv(glGetUniformLocation(m_id, label), 1, transpose, fm) );
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