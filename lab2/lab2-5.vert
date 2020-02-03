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
    mat3 normalMatrix1 = mat3(myMatrix);
    mat3 normalMatrix3 = mat3(view);
    vec3 transformedNormal = normalMatrix3 * normalMatrix1 * inNormal;
    const vec3 light = vec3(0.58, 0.58, 0.58);
    float shade;
    shade = dot(normalize(transformedNormal), light);
	exColor = vec3(shade);
	texcoord = inTexCoord;
    gl_Position = projectionMatrix * view * myMatrix * vec4(in_Position, 1.0);
}
