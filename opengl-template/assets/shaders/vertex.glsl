
#version 330

layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec2 inTextureCoords;
layout(location = 2) in vec3 inNormal;


uniform mat4 unifProjectionTransform;
uniform mat4 unifViewTransform;
uniform mat4 unifWorldTransform;
out     vec2 textureCoords;
out     vec3 normal;
out     vec4 position;

void main()
{
	textureCoords = inTextureCoords;
	normal = inNormal;
	position = inPosition;
	gl_Position = unifProjectionTransform * unifViewTransform * unifWorldTransform * inPosition;
}
