#version 150

in vec3 exNormal;
out vec4 out_Color;
uniform float visibility;
uniform int sharkID;

void main(void)
{
	vec4 skyColor = vec4(0.2, 0.6353, 0.9255, 1.0);
	vec4 baseColor;

	if (sharkID == 0) {
		baseColor = vec4(0.7529, exNormal.y, 0.502, 1.0);
	} else {
		baseColor = vec4(0.4627, exNormal.y, 0.0, 1.0);
	}
	
	float fogFactor = clamp(visibility + 0.2, 0.0, 1.0);
	vec4 fragColor = mix(baseColor, skyColor, fogFactor);
	out_Color = fragColor;
}
