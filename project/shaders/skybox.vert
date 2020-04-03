#version 150

in  vec3 inPosition;
in  vec3 inNormal;

uniform mat4 transform;
uniform mat4 projectionMatrix;
uniform mat4 view;

out vec3 exColor;
in vec2 inTexCoord;
out vec2 texcoord;

void main(void)
{
    gl_Position = projectionMatrix * view * transform * vec4(inPosition, 1.0);
    exColor = inNormal;
	texcoord = inTexCoord;
}
