#version 150

in  vec3 in_Position;
in  vec3 inNormal;
uniform mat4 myMatrix;
uniform mat4 projectionMatrix;
uniform mat4 view;
out vec3 exColor;
in vec2 inTexCoord;
out vec2 texcoord;

void main(void)
{
 	gl_Position = projectionMatrix * view * myMatrix * vec4(in_Position, 1.0);
	//gl_Position = vec4(in_Position, 1.0);
	exColor = inNormal;
	texcoord = inTexCoord;
}
