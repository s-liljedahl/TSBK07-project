#version 150

in  vec3 in_Position;
in  vec3 inNormal;
out vec3 exColor;

void main(void)
{
	// const vec3 light = vec3(0.58, 0.58, 0.58);
	// const vec3 light = vec3(1.0, 1.0, 1.0);
	// float shade;
	// shade = dot(normalize(inNormal), light);
	// exColor = vec3(shade);
	exColor = inNormal;
	gl_Position = vec4(in_Position, 1.0);
}
