#version 400 core

in vec4 VertPosition;
in vec4 VertNormal;
in vec4 VertColor;
in vec4 VertTexture;

out vec4 FragColor;

uniform sampler2D tex;

void main()
{
   vec4 color = texture(tex, VertTexture.xy);
   FragColor = vec4(color.xyz,1.0);	
}
