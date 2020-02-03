#version 150

in  vec3 in_Position;
in  vec3 inNormal;
uniform mat4 myMatrix;
uniform mat4 projectionMatrix;
uniform mat4 view;
out vec3 exColor;

void main(void)
{
    exColor = inNormal;
    gl_Position = projectionMatrix * view * myMatrix * vec4(in_Position, 1.0);
}
