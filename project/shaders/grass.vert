#version 150

in vec3 inPosition;
in vec3 inNormal;
uniform mat4 myMatrix;
out vec3 normal;

void main(void)
{
    gl_Position = myMatrix * vec4(inPosition, 1.0);
    normal = inNormal;
}
