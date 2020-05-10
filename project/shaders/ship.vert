#version 150

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

uniform mat4 projMatrix;
uniform mat4 shipMatrix;

out vec3 normal;
out vec4 vertPostion;

void main(void)
{ 
 	gl_Position = projMatrix * shipMatrix * vec4(inPosition, 1.0);
  normal = inNormal;
  vertPostion = vec4(inPosition, 1.0);
}