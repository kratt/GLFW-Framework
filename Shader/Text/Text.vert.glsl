#version 400 core

#define VERT_POSITION	0
#define VERT_NORMAL     1
#define VERT_COLOR		2
#define VERT_TEXTURE    3

uniform mat4x4 matModel;
uniform mat4x4 matView;
uniform mat4x4 matProjection;
uniform bool faceToCamera = false;
uniform vec2 textDims = vec2(1.0f);
uniform float border = 0.0f;
uniform float gapToBorder = 0.0f;

layout(location = VERT_POSITION) in vec4 Position;
layout(location = VERT_NORMAL)   in vec4 Normal;
layout(location = VERT_COLOR)    in vec4 Color;
layout(location = VERT_TEXTURE)  in vec4 Texture;

out vec4 VertPosition;
out vec4 VertNormal;
out vec4 VertColor;
out vec4 VertTexture;

out vec2 VertBorderCoordsX;
out vec2 VertBorderCoordsY;

void main()
{	   
    VertPosition = Position; 
    VertNormal   = Normal;
	VertColor    = Color;
	VertTexture  = Texture;
	
	mat4 matViewNew = matView;
	mat4 matModelNew = matModel;
	
	if(faceToCamera)
	{
		matViewNew = matView * matModel;
		matViewNew[0].xyz = vec3(1,0,0);
		matViewNew[1].xyz = vec3(0,1,0);
		matViewNew[2].xyz = vec3(0,0,1);
		
		matModelNew[3] = vec4(0,0,0,1);
	}
	
	// shift text coords if we have a border (consider gap to boder)
	if(border > 0.0f)
	{
		float totalWidth  = textDims.x + 2.0f * (border + gapToBorder);
		float totalHeight = textDims.y + 2.0f * (border + gapToBorder);
		
		float tMinX = (border + gapToBorder) / totalWidth;
		float tMaxX = 1.0f - tMinX;
		
		float tMinY = (border + gapToBorder) / totalHeight;
		float tMaxY = 1.0f - tMinY;
		
		float textCoordX = (Texture.x - tMinX) / (tMaxX - tMinX);
		float textCoordY = (Texture.y - tMinY) / (tMaxY - tMinY);
		VertTexture = vec4(textCoordX, textCoordY, 0.0, 0.0);
		
		float borderStartX = border / totalWidth;
		float borderEndX   = 1.0f - borderStartX;
		
		float borderStartY = border / totalHeight;
		float borderEndY   = 1.0f - borderStartY;
		
		VertBorderCoordsX.x = (borderStartX - tMinX) / (tMaxX - tMinX);
		VertBorderCoordsX.y = (borderEndX - tMinX)   / (tMaxX - tMinX);
		
		VertBorderCoordsY.x = (borderStartY - tMinY) / (tMaxY - tMinY);
		VertBorderCoordsY.y = (borderEndY - tMinY)   / (tMaxY - tMinY);
	}
	
    gl_Position = matProjection * matViewNew * matModelNew * vec4(Position.xyz, 1);
}






