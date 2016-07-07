#version 400 core

in vec4 VertPosition;
in vec4 VertNormal;
in vec4 VertColor;
in vec4 VertTexture;

in vec2 VertBorderCoordsX;
in vec2 VertBorderCoordsY;

out vec4 FragColor;

uniform sampler2D tex;
uniform vec4 textColor   = vec4(1.0f);
uniform vec4 borderColor = vec4(1.0f);

void main()
{
	float alpha = 0.0f;
	vec3 color = vec3(1.0f);
	
	// text
	if(VertTexture.x >= 0.0f && VertTexture.x <= 1.0f &&
	   VertTexture.y >= 0.0f && VertTexture.y <= 1.0f)
   {
	  float texAlpha = texture(tex, vec2(VertTexture.x,  1.0f-VertTexture.y)).x;
	  alpha = min(texAlpha, textColor.a);
	  color = textColor.xyz;
   }
   // gap between text and border
   else if(VertTexture.x > VertBorderCoordsX.x && VertTexture.x < VertBorderCoordsX.y &&
           VertTexture.y > VertBorderCoordsY.x && VertTexture.y < VertBorderCoordsY.y){
	   alpha = 0.0f;
   }
   // border
   else{
	   alpha = borderColor.w;
	   color = borderColor.xyz;	   
   }
   
   alpha = max(0.0, alpha);
   FragColor = vec4(color, alpha);
}
