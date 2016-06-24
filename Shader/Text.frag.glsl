#version 400 core

in vec4 VertPosition;
in vec4 VertNormal;
in vec4 VertColor;
in vec4 VertTexture;

out vec4 FragColor;

uniform sampler2D tex;

void main()
{
   float alpha = texture(tex, vec2(VertTexture.x, 1.0f - VertTexture.y)).x;
   vec3 color = vec3(1.0f, 1.0f, 1.0f);
   
   //alpha = alpha > 0 ? 1.0: 0.0f;
   
   alpha = max(0.1, alpha);
   FragColor = vec4(color, alpha);	
}
