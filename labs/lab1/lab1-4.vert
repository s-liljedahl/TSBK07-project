#version 150

in  vec3 in_Position;
in  vec3 inNormal;
out vec3 exColor;

void main(void)
{
	exColor = inNormal;
	gl_Position = vec4(in_Position, 1.0);
}
