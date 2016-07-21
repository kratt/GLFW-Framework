#version 400 core

#define VERT_POSITION	0
#define VERT_NORMAL     1
#define VERT_COLOR		2
#define VERT_TEXTURE    3

uniform mat4x4 matModel;
uniform mat4x4 matView;
uniform mat4x4 matProjection;

uniform int windowWidth  = 1;
uniform int windowHeight = 1;
uniform vec2 trans = vec2(0.0f);
uniform bool render3D = false;

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
	
	if(render3D)
	{
		mat4 matViewNew = matView;
		mat4 matModelNew = matModel;

		matViewNew = matView * matModel;
		matViewNew[0].xyz = vec3(1,0,0);
		matViewNew[1].xyz = vec3(0,1,0);
		matViewNew[2].xyz = vec3(0,0,1);
		
		matModelNew[3] = vec4(0,0,0,1);
		
		gl_Position = matProjection * matViewNew  * matModelNew * vec4(Position.xy, 0, 1);
	}
	else
	{
		vec2 pos = Position.xy + trans;
		pos = (pos / vec2(float(windowWidth), float(windowHeight))) * 2.0f - 1.0f;
			
	   gl_Position = vec4(pos, 0.0, 1);
		
	}

}






