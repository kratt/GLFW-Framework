#version 400 core

in vec4 VertPosition;
in vec4 VertNormal;
in vec4 VertColor;
in vec4 VertTexture;

out vec4 FragColor;

uniform sampler2D texAtlas;

void main()
{
   float alpha = texture(texAtlas, vec2(VertTexture.x,  VertTexture.y)).x;
	
   FragColor = vec4(vec3(1), alpha);
}
