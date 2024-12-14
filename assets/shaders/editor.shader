//Vertex

#version 460

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoord;

//Projection Matrix
uniform mat4 projectionMatrix = mat4(0.0);
//Transformation Matrix
uniform mat4 modelMatrix = mat4(1.0);
//Camera Transformation Matrix
uniform mat4 viewMatrix = mat4(1.0);

out vec2 TexCoord;

void main()
{
	vec4 newPos = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
	gl_Position = newPos;
	TexCoord = textureCoord;
}

//Fragment

#version 460

layout(location = 0) out int objectIndex;

in vec2 TexCoord;

uniform sampler2D texture0;

uniform int index;

uniform bool hasTexture = false;
uniform bool canChromaKey = false;

uniform vec3 chromaKey = vec3(0.0, 0.0, 0.0);
vec4 colour;

void main()
{
	if (hasTexture) 
	{
		colour = texture(texture0, TexCoord);
	}
	else 
	{
		colour = vec4(1.0, 1.0, 1.0, 1.0);
	}
	if (canChromaKey) 
	{
		float r = abs(colour.r - chromaKey.r);
		float g = abs(colour.g - chromaKey.g);
		float b = abs(colour.b - chromaKey.b);
		if (r < 0.1 && g < 0.1 && b < 0.1) 
		{
			discard;
		}
	}
	objectIndex = index;
}