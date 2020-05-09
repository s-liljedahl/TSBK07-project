#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in vec2 inTexCoord;
out vec2 texCoord;
out vec4 vertPostion;
out vec3 exColor;

out float visibility;

// NY
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;

uniform vec3 cameraPos;

float getFogFactor(float d)
{
    const float FogMax = 70.0;
    const float FogMin = 1.0;
    if (d>=FogMax) return 1;
    if (d<=FogMin) return 0;
    return 1 - (FogMax - d) / (FogMax - FogMin);
}

void main(void)
{
	float dist = distance(cameraPos, inPosition.xyz);
	visibility = getFogFactor(dist);


	mat3 normalMatrix1 = mat3(mdlMatrix);
	texCoord = inTexCoord;

	const vec3 light = vec3(0.5, 1.0, 0.3);
	float shade = dot(normalize(inNormal), normalize(light));
	exColor = vec3(shade);
	vertPostion = vec4(inPosition, 1.0);
	gl_Position = projMatrix * mdlMatrix * vec4(inPosition, 1.0);
}
