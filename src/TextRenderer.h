// 22.07.2016
// author: Julian Kratt

#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H


#include "TextString.h"

#include <string> 
#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class Shader;
class VertexBufferObjectAttribs;
class TextureAtlas;

class TextRenderer
{

public:
	TextRenderer();
	~TextRenderer();

	void render(const std::string &text, const glm::vec2 &pos, unsigned int fontSize = 12, const glm::vec4 &color = glm::vec4(1.0f), const std::string &font = "calibril.ttf");
	void render(const std::string &text, const glm::vec3 &pos, const glm::vec2 &dims, const glm::vec4 &color = glm::vec4(1.0f), const std::string &font = "calibril.ttf");

	// singelton
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


#endif
