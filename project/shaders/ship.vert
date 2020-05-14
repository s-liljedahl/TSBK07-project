#version 150

in vec2 inTexCoord;
in vec3 inPosition;
in vec3 inNormal;

uniform mat4 projMatrix;
uniform mat4 shipMatrix;

out vec3 normal;
out vec3 exColor;
out vec3 vertPosition;

void main(void)
{ 
  const vec3 light = vec3(0.5, 1.0, 0.3);
	float shade = dot(normalize(inNormal), normalize(light));
	exColor = vec3(shade);

 	gl_Position = projMatrix * shipMatrix * vec4(inPosition, 1.0);
  normal = inNormal;
  vertPosition = inPosition;
}