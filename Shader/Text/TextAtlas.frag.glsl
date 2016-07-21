#version 400 core

in vec4 VertPosition;
in vec4 VertNormal;
in vec4 VertColor;
in vec4 VertTexture;

in vec2 VertBorderCoordsX;
in vec2 VertBorderCoordsY;

out vec4 FragColor;

uniform sampler2D texAtlas;

void main()
{
   float alpha = texture(texAtlas, vec2(VertTexture.x,  1.0f-VertTexture.y)).x;
	
   FragColor = vec4(vec3(1), alpha);
}
