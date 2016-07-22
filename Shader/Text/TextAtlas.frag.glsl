#version 400 core

in vec4 VertPosition;
in vec4 VertNormal;
in vec4 VertColor;
in vec4 VertTexture;

out vec4 FragColor;

uniform sampler2D texAtlas;
uniform vec4 textColor = vec4(1.0f);

void main()
{
   float alpha = texture(texAtlas, vec2(VertTexture.x,  VertTexture.y)).x;
	
   alpha = min(alpha, textColor.a);
	
   FragColor = vec4(textColor.xyz, alpha);
}
