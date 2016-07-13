#version 400 core

in vec4 VertPosition;
in vec4 VertNormal;
in vec4 VertColor;
in vec4 VertTexture;

out vec4 FragColor;

uniform sampler2D tex;

const float spread = 16.0f;
const float scale = 0.1;
const float smoothing = 0.25f / (spread * scale) ;
/*
void main()
{
   vec3 color = vec3(1.0f, 1.0f, 1.0f);
   float distance = 1.0-texture(tex, vec2(VertTexture.x, 1.0f - VertTexture.y)).x;
	
   float alpha = smoothstep(0.5 - smoothing, 0.5 + smoothing, distance);
   FragColor = vec4(vec3(1), alpha);
}
*/



void main()
{
	// get the alpha value from the distance field texture
    float distance = 1-texture(tex, vec2(VertTexture.x, 1.0f - VertTexture.y)).x;

	//distance = max(0.1, distance);
	
	float alpha = smoothstep(0.5f-smoothing,0.5f+smoothing, distance);
	
	FragColor = vec4(vec3(1), alpha );
	
	FragColor = vec4(vec3(distance), 1);
}

/*
void main() {
	
	// vec3 color = vec3(1.0f, 1.0f, 1.0f);
	// float u_gamma = 0.1;
    // float dist = texture(tex, vec2(VertTexture.x, 1.0f - VertTexture.y)).x;
    // float alpha = smoothstep(color - u_gamma, color + u_gamma, dist);
    // gl_FragColor = vec4(color.rgb, alpha);
}
*/






