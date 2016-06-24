#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

//https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Text_Rendering_01
//https://gitlab.com/wikibooks-opengl/modern-tutorials/blob/master/text01_intro/text.cpp
//https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Text_Rendering_02

#include "TextString.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <string> 

class Shader;
class VertexBufferObjectAttribs;
class Texture;


class TextRenderer {

public:
	TextRenderer();

	void renderText(int x, int y, const char *text);
	
	void render(TextString* text);


	static TextRenderer* instance();

protected:
	static TextRenderer* s_texRendererInstance;

private:
	void init();

	FT_Library ft;
	FT_Face face;

	Shader* m_shaderText;
	VertexBufferObjectAttribs* m_vboQuad;

	Texture *m_texTest;
};

#endif
