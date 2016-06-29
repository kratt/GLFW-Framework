#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

//https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Text_Rendering_01
//https://gitlab.com/wikibooks-opengl/modern-tutorials/blob/master/text01_intro/text.cpp
//https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Text_Rendering_02

#include "TextString.h"
#include <string> 
#include <vector>

class Shader;
class VertexBufferObjectAttribs;
class Texture;


class TextRenderer {

public:
	TextRenderer();
	~TextRenderer();

	void render(const std::string &text, int x, int y, int fontSize = 24, const std::string &font = "Calibri");
	void render3d(const std::string &text, glm::vec3 pos, int fontSize = 24, const std::string &font = "Calibri");

	static TextRenderer* instance();

protected:
	static TextRenderer* s_texRendererInstance;

private:
	void init();
	
	TextString* getTextString(const std::string &text, const std::string &font, int fontSize);

	void renderSdf(TextString* text);


	Shader* m_shaderText;
	Shader* m_shaderTextSdf;
	VertexBufferObjectAttribs* m_vboQuad;

	std::vector<TextString*> m_textStrings;

	// maps to store bitmaps. font -> fontSize -> textString obj. 
	//std::map<std::string /*font*/, std::map<int /*font size*/, std::map<std::string /*text*/ ,TextString*> > > m_textStrings;
};

#endif
