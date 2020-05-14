#version 150

in vec3 inPosition;
in vec3 inNormal;
uniform mat4 fishMatrix;
uniform mat4 projMatrix;
uniform mat4 view;
out vec3 exNormal;
out vec3 exColor;

void main(void)
{
	const vec3 light = vec3(0.5, 1.0, 0.3);
	float shade = dot(normalize(inNormal), normalize(light));
	exColor = vec3(shade);
 	gl_Position = projMatrix * fishMatrix * vec4(inPosition, 1.0);
	exNormal = inNormal;
}
