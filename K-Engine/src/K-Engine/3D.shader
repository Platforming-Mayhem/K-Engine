//Vertex

#version 460

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoord;

out vec2 TexCoord;

//Projection Matrix
uniform mat4 projectionMatrix = mat4(0.0);
//Transformation Matrix
uniform mat4 modelMatrix = mat4(1.0);
//Camera Transformation Matrix
uniform mat4 viewMatrix = mat4(1.0);

void main()
{
	vec4 newPos = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
	gl_Position = newPos;
	TexCoord = textureCoord;
}

//Fragment

#version 460

layout(location = 0) out vec4 colour;

in vec2 TexCoord;

uniform sampler2D tex;

float LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0; // back to NDC 
	return (2.0 * 0.1 * 1000) / (1000 + 0.1 - z * (1000 - 0.1));
}

void main()
{
	colour = texture(tex, TexCoord);
	if (colour.a < 0.5) 
	{
		discard;
	}
}