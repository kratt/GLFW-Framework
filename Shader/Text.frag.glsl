#version 400 core

in vec4 VertPosition;
in vec4 VertNormal;
in vec4 VertColor;
in vec4 VertTexture;

out vec4 FragColor;

void main()
{
   vec4 color = VertColor;
   FragColor = vec4(color);	
}
