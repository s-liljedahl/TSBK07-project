#version 150

in vec3 inPosition;
in vec3 inNormal;
uniform mat4 sphereMatrix;
uniform mat4 moveMatrix;
out vec3 exNormal;

void main(void)
{
 	gl_Position = sphereMatrix * moveMatrix * vec4(inPosition, 1.0);
	exNormal = inNormal;
	}
