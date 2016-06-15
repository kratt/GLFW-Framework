//Author: Sören Pirk
//Date: 22.01.2013

#version 400 core

in vec4 VertPosition;
in vec4 VertNormal;
in vec4 VertColor;
in vec4 VertTexture;
out vec4 FragColor;

uniform vec3 lightPos;

void main()
{
   vec4 color = VertColor;
   FragColor = vec4(color);	
}
