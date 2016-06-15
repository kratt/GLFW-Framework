// Computer Graphics and Media Informatics
// University of Konstanz
// 2015

#ifndef SHADER
#define SHADER

#include "Common.h"
#include "Global.h"

/*
-----------------------------------------------------------------------------
Shader.h
-----------------------------------------------------------------------------

This class represents a set of shader programs.
A short usage description:

1.	Shader(const GLchar *pathToVertexShader, const GLchar *pathToFragmentShader)
	Creates a shader with a vertex and fragment program attached. Parameters are
	the localtions of the vertex and fragment shader code files (text files).

2.  bind()
	Activates the shader. All upcoming geometry is passed through the vertex and
	fragment program from the shader. The vertex shader is called for each vertex
	and the fragment shader for each fragment (potential pixel).

3.	set*()
	Possible methods: setMatrix() / setf() / seti() / set[2/3/4][f/i][v]()
	Binds uniform variables to a shader. Uniform variables are a possibility to
	transfer parameters from the CPU to the GPU. Uniform parameters are visible
	in all shaders.
	Example:	CPU: shader->setf("floatVar", 1.0f);
				GPU: uniform float floatVar;

4.  release()
	Deactivates the shader.

-----------------------------------------------------------------------------
*/

class Shader
{
	private:		
        GLuint m_id;

		const GLchar *m_vFileName;
        const GLchar *m_cFileName;
        const GLchar *m_eFileName;
		const GLchar *m_gFileName;
		const GLchar *m_fFileName;  

        GLuint m_vertProg;
        GLuint m_contProg;
        GLuint m_evalProg;
        GLuint m_geomProg;
        GLuint m_fragProg;
        
        GLuint m_refreshTime;

		const GLchar *readFile(const GLchar *fileName);
        void checkFile(const GLchar *fileName, GLuint type);        
        GLuint compile(const GLchar *source, GLuint type);
        void cleanUp();        

    public:       
        Shader();
        Shader(const GLchar *vFileName, const GLchar *fFileName);
        Shader(const GLchar *vFileName, const GLchar *gFileName, const GLchar *fFileName);
        virtual ~Shader();

		void bind() const;
		void release() const;  
        void link() const;

        void attachVertexShader(const GLchar *fileName);
        void attachControlShader(const GLchar *fileName);
        void attachEvaluationShader(const GLchar *fileName);
        void attachGeometryShader(const GLchar *fileName);
        void attachFragmentShader(const GLchar *fileName);   

        void bindAttribLocation(const GLchar *label, GLuint attribID);


        void seti(const GLchar* label, GLint arg);
        void setf(const GLchar* label, GLfloat arg);
        void set2i(const GLchar* label, GLint arg1, GLint arg2);
        void set2f(const GLchar* label, GLfloat arg1, GLfloat arg2);
        void set3i(const GLchar* label, GLint arg1, GLint arg2, GLint arg3);
        void set3f(const GLchar* label, GLfloat arg1, GLfloat arg2, GLfloat arg3);
        void set3f(const GLchar* label, const glm::vec3 &v);
        void set4i(const GLchar* label, GLint arg1, GLint arg2, GLint arg3, GLint arg4);
        void set4f(const GLchar* label, GLfloat arg1, GLfloat arg2, GLfloat arg3, GLfloat arg4);
        void set3iv(const GLchar* label, const GLint* args);
        void set3fv(const GLchar* label, const GLfloat* args);
        void setMatrix(const GLchar* label, const GLfloat* m, GLboolean transpose = GL_FALSE);
        void setMatrix(const GLchar* label, const GLdouble* m, GLboolean transpose = GL_FALSE);
        void setMatrix(const GLchar* label, const glm::mat4x4 &mat, GLboolean transpose = GL_FALSE);
        void setMatrix(const GLchar* label, const glm::mat3x3 &mat, GLboolean transpose = GL_FALSE);

        GLuint id() const;

		void bindAttribLocations();
};

#endif

