#version 400 core

#define VERT_POSITION	0
#define VERT_NORMAL     1
#define VERT_COLOR		2
#define VERT_TEXTURE    3

uniform mat4x4 matModel;
uniform mat4x4 matView;
uniform mat4x4 matProjection;
uniform mat4x4 matLightView;

uniform vec4 clipPlane;

layout(location = VERT_POSITION) in vec4 Position;
layout(location = VERT_NORMAL)   in vec4 Normal;
layout(location = VERT_COLOR)    in vec4 Color;
layout(location = VERT_TEXTURE)  in vec4 Texture;

out vec3 VertPosition;
out vec3 VertNormal;
out vec4 VertColor;
out vec4 VertTexture;
out vec4 VertShadowCoord;

void main()
{	   
    VertPosition    = Position.xyz; 
    VertNormal      = vec4(transpose(inverse(matModel)) * vec4(Normal.xyz, 1)).xyz;    
	VertColor       = Color;
	VertTexture     = Texture;
    VertShadowCoord = matLightView * matModel * vec4(Position.xyz, 1);
	
    gl_Position = matProjection * matView * matModel * vec4(Position.xyz, 1);
}
