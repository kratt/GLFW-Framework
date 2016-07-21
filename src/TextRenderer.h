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
class TextureAtlas;


class TextRenderer {

public:
	TextRenderer();
	~TextRenderer();

	void render(const std::string &text, glm::vec2 pos, unsigned int fontSize = 12, const std::string &font = "calibril.ttf");
	void render(const std::string &text, glm::vec3 pos, glm::vec2 dims, const std::string &font = "calibril.ttf");

	static TextRenderer* instance();

protected:
	static TextRenderer* s_texRendererInstance;

private:
	void init();

	TextureAtlas* getTextureAtlas(const std::string &font, unsigned int fontSize);
	TextString* getTextString(TextureAtlas* atlas, const std::string &text);
	TextString* createTextString(TextureAtlas *atlas, const std::string &text);

private:
	unsigned int m_maxNumPriority;
	unsigned int m_fontSize3D;

	Shader* m_shader;
	std::string m_fontPath;
	std::vector<TextureAtlas*> m_atlases;
	std::vector<TextString*> m_textStrings;
};



//
//class TextRenderer {
//
//public:
//	TextRenderer();
//	~TextRenderer();
//
//	void render_with_tex_atlas(const std::string &text, glm::vec2 pos, int fontSize);
//
//
//	// 2d render functions
//	void render(const std::string &text, glm::vec2 pos, int fontSize = 24, glm::vec4 &textColor = glm::vec4(1.0f), const std::string &font = "calibril");
//	void render(const std::string &text, glm::vec2 pos, int border, int gapToBorder, int fontSize = 24, glm::vec4 &textColor = glm::vec4(1.0f), glm::vec4 &borderColor = glm::vec4(1.0f), const std::string &font = "Calibri");
//
//	// 3d render functions
//	void render(const std::string &text, glm::vec3 pos, glm::vec2 dims, glm::vec4 &textColor = glm::vec4(1.0f), const std::string &font = "Calibri");
//	void render(const std::string &text, glm::vec3 pos, glm::vec2 dims, float border, float gapToBorder, glm::vec4 &textColor = glm::vec4(1.0f), glm::vec4 &borderColor = glm::vec4(1.0f), const std::string &font = "Calibri");
//
//
//	void renderSdf(const std::string &text, glm::vec2 pos, int fontSize = 24, const std::string &font = "Calibri");
//
//	static TextRenderer* instance();
//	std::vector<TextString*> m_textStrings;
//
//protected:
//	static TextRenderer* s_texRendererInstance;
//
//private:
//	void init();
//	void initTextureAtlas(const std::string &font, unsigned int fontSize);
//
//	void render2d(const std::string &text, glm::vec2 pos, glm::vec4 textColor, glm::vec4 borderColor, int border, int gapToBorder, int fontSize, const std::string &font);
//	void render3d(const std::string &text, glm::vec3 pos, glm::vec2 dims, glm::vec4 textColor, glm::vec4 borderColor, float border, float gapToBorder, const std::string &font);
//
//	TextString* getTextString(const std::string &text, const std::string &font, int fontSize);
//
//	Shader* m_shaderText;
//	Shader* m_shaderTextSdf;
//	Shader* m_shaderTextAtlas;
//	VertexBufferObjectAttribs* m_vboQuad;
//
//	VertexBufferObjectAttribs* m_vboQuadAtlas;
//
//	std::string m_fontPath;
//	std::vector<TextureAtlas*> m_atlases;
//
//	int m_fontSize3D;
//};

#endif
