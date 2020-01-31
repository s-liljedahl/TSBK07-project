#version 150

in  vec3 in_Position;
in  vec3 inNormal;
uniform mat4 myMatrix;
out vec3 exColor;

void main(void)
{
 	gl_Position = myMatrix * vec4(in_Position, 1.0);
	//gl_Position = vec4(in_Position, 1.0);
	exColor = inNormal;
}
