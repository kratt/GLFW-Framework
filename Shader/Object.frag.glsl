/*
#version 400 core

in vec3 VertPosition;
in vec3 VertNormal;
in vec4 VertColor;
in vec4 VertTexture;
in vec4 VertShadowCoord;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 camPos;
uniform sampler2D texKd;


const float PI = 3.14f;

float attenuation(float r, float f, float d) 
{
  float denom = d / r + 1.0;
  float attenuation = 1.0 / (denom*denom);
  float t = (attenuation - f) / (1.0 - f);
  return max(t, 0.0);
}

float phongSpecular(vec3 lightDirection, vec3 viewDirection, vec3 surfaceNormal, float shininess) 
{
  vec3 R = -reflect(lightDirection, surfaceNormal);
  return pow(max(0.0, dot(viewDirection, R)), shininess);
}

float orenNayarDiffuse(vec3 lightDirection, vec3 viewDirection, vec3 surfaceNormal, float roughness, float albedo) {
  
  float LdotV = dot(lightDirection, viewDirection);
  float NdotL = dot(lightDirection, surfaceNormal);
  float NdotV = dot(surfaceNormal, viewDirection);

  float s = LdotV - NdotL * NdotV;
  float t = mix(1.0, max(NdotL, NdotV), step(0.0, s));

  float sigma2 = roughness * roughness;
  float A = 1.0 + sigma2 * (albedo / (sigma2 + 0.13) + 0.5 / (sigma2 + 0.33));
  float B = 0.45 * sigma2 / (sigma2 + 0.09);

  return albedo * max(0.0, NdotL) * (A + B * s / t) / PI;
}


void main(void) 
{
  vec3 resColor = vec3(0.0);

  // calculate attenuation
  float lightDistance = length(lightPos - VertPosition);
  float falloff = attenuation(1020, 0.05, lightDistance);

  // difuse and specular color
  vec3 diffuseColor = texture(texKd, VertTexture.st).rgb;
  float specularStrength = 0.5;
  float specularScale = 0.65;
  float shininess = 20.0;
  float roughness = 1.0;
  float albedo = 0.95;

  // light direction
  vec3 L = normalize(lightPos - VertPosition);  // light direction
  vec3 V = normalize(camPos - VertPosition); // eye direction
  vec3 N = normalize(VertNormal);

  float specular = specularStrength * phongSpecular(L, V, N, shininess) * specularScale * falloff;

  vec3 diffuse = diffuseColor * orenNayarDiffuse(L, V, N, roughness, albedo) * falloff;
  vec3 ambient = vec3(0.1);

  resColor = diffuse + ambient + specular;
  resColor = resColor;

  FragColor = vec4(resColor, 1);
}
*/



#version 400 core

in vec4 VertPosition;
in vec4 VertNormal;
in vec4 VertColor;
in vec4 VertTexture;
in vec4 VertShadowCoord;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 camPos;
uniform int applyShadow;
uniform float shadowIntensity;

uniform sampler2D shadowMapBlurred;
uniform sampler2D shadowMap;
uniform sampler2D texKd;

uniform int isSelected = 0;


float VSM(vec4 smcoord)
{
	vec3 coords = smcoord.xyz / smcoord.w ;    

    if(smcoord.z < 1)
       return 1;

    float depth =  coords.z;
    
    vec4 depthBlurrred = texture(shadowMapBlurred, coords.xy);

    float depthSM = depthBlurrred.x;
	float sigma2  = depthBlurrred.y;

    float realDepth = texture(shadowMap, coords.xy).x;

	sigma2 -= depthSM * depthSM;

	float bias = 0.000001;

	float dist = depth - depthSM;
	float P = sigma2 / ( sigma2 + dist * dist);
 	float lit = max( P, ( depth - bias ) <= depthSM ? 1.0 : 0.0);
	lit = min(1.0, lit);   

    return mix(shadowIntensity, 1.0, lit);
}

void main()
{
    vec4 color = VertColor;

    float shadow = 1.0;
    if(applyShadow == 1)    
    {
       shadow = VSM(VertShadowCoord);
    }
	
    vec3 P = VertPosition.xyz;
    vec3 N = normalize(VertNormal.xyz);
    vec3 L = normalize(lightPos.xyz - P);
    vec3 V = normalize(camPos.xyz-P);
    vec3 H = normalize(L+V);

    float d = max(dot(N, L), 0);
    float s = pow(max(dot(N, H), 0), 128);

    if(d <= 0)
        s = 0;
	
    vec3 lColor      = vec3(1, 1, 1);
    vec3 matAmbient  = vec3(0.0, 0.0, 0.0);
    vec3 matDiffuse  = vec3(1, 1, 1);
    vec3 matSpecular = vec3(1, 1, 1);

    if(isSelected == 1)
        matDiffuse = vec3(1, 0, 0);

    vec3 ambient  = matAmbient  * lColor;
    vec3 diffuse  = matDiffuse  * lColor * d * 1.0;
    vec3 specular = matSpecular * lColor * s * 0.0;

	vec3 kdColor = texture(texKd, vec2(VertTexture.x, VertTexture.y)).xyz;
	
	
    vec4 finalColor = vec4(kdColor, 1) * vec4(diffuse + ambient + specular, 1);
    finalColor.xyz *= shadow;

    FragColor = vec4(finalColor.xyz, 1.0);	
}


