// 21.06.2016
// author: Julian Kratt

#ifndef RENDER_UTILS_H
#define RENDER_UTILS_H

#include "Global.h"

class Shader;
class VertexBufferObjectAttribs;

class RenderUtils
{
private:
	RenderUtils();
	~RenderUtils();
	RenderUtils(const RenderUtils&){};

public:
	static RenderUtils* instance();
	void renderTexture(int x, int y, int width, int height, GLuint texId);

protected:
	// static instance / singelton
	static RenderUtils* m_instance;

	Shader* m_shaderTex2D;
	VertexBufferObjectAttribs* m_vboQuad;

private:
	void init();
};


#endif
