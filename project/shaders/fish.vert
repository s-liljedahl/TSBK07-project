#version 150

in vec3 inPosition;
in vec3 inNormal;
uniform mat4 fishMatrix;
uniform mat4 projMatrix;
uniform mat4 view;
out vec3 exNormal;

void main(void)
{
 	gl_Position = projMatrix * fishMatrix * vec4(inPosition, 1.0);
	exNormal = inNormal;
	}
