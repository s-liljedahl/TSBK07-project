#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in vec2 inTexCoord;
out vec2 texCoord;

out vec3 exColor;

// NY
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;

void main(void)
{
	mat3 normalMatrix1 = mat3(mdlMatrix);
	texCoord = inTexCoord;

	const vec3 light = vec3(0.3, 0.3, 0.3);
	float shade = dot(normalize(inNormal), normalize(light));
	exColor = vec3(shade);

	gl_Position = projMatrix * mdlMatrix * vec4(inPosition, 1.0);
}
