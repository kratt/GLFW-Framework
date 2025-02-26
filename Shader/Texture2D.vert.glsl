#version 400 core

#define VERT_POSITION	0
#define VERT_NORMAL     1
#define VERT_COLOR		2
#define VERT_TEXTURE    3

uniform mat4x4 matModel;
uniform mat4x4 matView;
uniform mat4x4 matProjection;

uniform vec2 windowSize = vec2(1.0f);
uniform vec2 texSize = vec2(1.0f);
uniform vec2 texStartPos = vec2(0.0f);


layout(location = VERT_POSITION) in vec4 Position;
layout(location = VERT_NORMAL)   in vec4 Normal;
layout(location = VERT_COLOR)    in vec4 Color;
layout(location = VERT_TEXTURE)  in vec4 Texture;

out vec4 VertPosition;
out vec4 VertNormal;
out vec4 VertColor;
out vec4 VertTexture;


void main()
{	   
	// determine offset and scaling for new texture coords
	float min_x = texStartPos.x / windowSize.x;
	float max_x = (texStartPos.x + texSize.x) / windowSize.x;
	
	float min_y = texStartPos.y / windowSize.y;
	float max_y = (texStartPos.y + texSize.y) / windowSize.y;

	float texCoordX = (Texture.x - min_x) / (max_x - min_x);
	float texCoordY = (Texture.y - min_y) / (max_y - min_y);
	
    VertPosition = Position; 
    VertNormal   = Normal;
	VertColor    = Color;
	VertTexture  = vec4(texCoordX, texCoordY, 0.0, 0.0);
	
    gl_Position = matProjection * matView * matModel * vec4(Position.xyz, 1);
}
