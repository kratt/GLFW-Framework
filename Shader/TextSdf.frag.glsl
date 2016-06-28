#version 400 core

in vec4 VertPosition;
in vec4 VertNormal;
in vec4 VertColor;
in vec4 VertTexture;

out vec4 FragColor;

uniform sampler2D tex;

const float smoothing = 1.0/64.0;

void main()
{
   vec3 color = vec3(1.0f, 1.0f, 1.0f);
   float distance = texture(tex, vec2(VertTexture.x, 1.0f - VertTexture.y)).x;
	
   float alpha = smoothstep(0.5 - smoothing, 0.5 + smoothing, distance);
   FragColor = vec4(vec3(0.3), 2.0);
}
