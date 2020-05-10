#version 150

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

uniform mat4 transform;
uniform mat4 projectionMatrix;
uniform mat4 view;

out vec2 texCoord;

void main(void)
{
    gl_Position = projectionMatrix * view * transform * vec4(inPosition, 1.0);
	texCoord = inTexCoord;
}
