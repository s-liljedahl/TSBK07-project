#version 150

in  vec3 in_Position;
in  vec3 inNormal;
uniform mat4 myMatrix;
uniform mat4 projectionMatrix;
uniform mat4 view;
out vec3 exColor;
in vec2 inTexCoord;
out vec2 texcoord;
out vec3 frag_position;

void main(void)
{
  gl_Position = projectionMatrix * view * myMatrix * vec4(in_Position, 1.0);
  exColor = mat3(myMatrix) * inNormal;
  texcoord = in_Position.xz;
  frag_position = vec3(myMatrix * vec4(in_Position, 1.0));

}
