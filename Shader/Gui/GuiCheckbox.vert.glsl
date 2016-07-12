#version 400 core

#define VERT_POSITION	0
#define VERT_NORMAL     1
#define VERT_COLOR		2
#define VERT_TEXTURE    3


layout(location = VERT_POSITION) in vec4 Position;
layout(location = VERT_NORMAL)   in vec4 Normal;
layout(location = VERT_COLOR)    in vec4 Color;
layout(location = VERT_TEXTURE)  in vec4 Texture;

uniform int windowWidth  = 1;
uniform int windowHeight = 1;

void main()
{	   
	// map pos to range [-1, 1]
	vec2 pos = (Position.xy / vec2(float(windowWidth), float(windowHeight))) * 2.0f - 1.0f;
	
	// flip vertical axis
	pos.y *= -1.0f;
	
   gl_Position = vec4(pos, 0.0, 1);
}





