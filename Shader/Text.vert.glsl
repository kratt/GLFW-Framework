//Author: Sören Pirk
//Date: 22.01.2013

#version 400 core

#define VERT_POSITION	0
#define VERT_NORMAL     1
#define VERT_COLOR		2
#define VERT_TEXTURE    3

uniform mat4x4 matModel;
uniform mat4x4 matView;
uniform mat4x4 matProjection;
uniform bool faceToCamera = false;

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
    VertPosition = Position; 
    VertNormal   = Normal;
	VertColor    = Color;
	VertTexture  = Texture;
	
	mat4 matViewNew = matView;
	mat4 matModelNew = matModel;
	
	if(faceToCamera)
	{
		matViewNew = matView * matModel;
		matViewNew[0].xyz = vec3(1,0,0);
		matViewNew[1].xyz = vec3(0,1,0);
		matViewNew[2].xyz = vec3(0,0,1);
		
		matModelNew[3] = vec4(0,0,0,1);
	}
	
    gl_Position = matProjection * matViewNew * matModelNew * vec4(Position.xyz, 1);
}
