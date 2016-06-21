#version 400 core

in vec4 VertPosition;
in vec4 VertNormal;
in vec4 VertColor;
in vec4 VertTexture;

out vec4 FragColor;

uniform sampler2D tex;

void main()
{
   if(VertTexture.x < 0.0 || VertTexture.x > 1.0f || 
      VertTexture.y < 0.0 || VertTexture.y > 1.0f)
	  {
		  discard;
	  }
	  
	
   vec4 color = texture(tex, VertTexture.xy);
   FragColor = vec4(color.xyz, 1.0);	
}
