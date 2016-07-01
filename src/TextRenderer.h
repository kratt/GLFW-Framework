#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

//https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Text_Rendering_01
//https://gitlab.com/wikibooks-opengl/modern-tutorials/blob/master/text01_intro/text.cpp
//https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Text_Rendering_02


//http://contourtextures.wikidot.com/
#include "TextString.h"
#include <string> 
#include <vector>
#include <glm/vec4.hpp>

class Shader;
class VertexBufferObjectAttribs;
class Texture;


class TextRenderer {

public:
	TextRenderer();
	~TextRenderer();

	void render(const std::string &text, glm::vec2 pos, int fontSize = 24, glm::vec4 &textColor = glm::vec4(1.0f), const std::string &font = "Calibri");
	void render(const std::string &text, glm::vec2 pos, int border, int gapToBorder, int fontSize = 24, glm::vec4 &textColor = glm::vec4(1.0f), glm::vec4 &borderColor = glm::vec4(1.0f), const std::string &font = "Calibri");


	void render(const std::string &text, glm::vec3 pos, int fontSize = 24, const std::string &font = "Calibri");



	void renderSdf(const std::string &text, glm::vec2 pos, int fontSize = 24, const std::string &font = "Calibri");

	static TextRenderer* instance();

protected:
	static TextRenderer* s_texRendererInstance;

private:
	void init();

	void render(const std::string &text, glm::vec3 pos, glm::vec4 textColor, glm::vec4 borderColor, int border, int gapToBorder, int fontSize, const std::string &font, bool render3D);
	
	TextString* getTextString(const std::string &text, const std::string &font, int fontSize);

	Shader* m_shaderText;
	Shader* m_shaderTextSdf;
	VertexBufferObjectAttribs* m_vboQuad;

	std::vector<TextString*> m_textStrings;
};

#endif
