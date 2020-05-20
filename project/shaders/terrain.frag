#version 150

in vec2 texCoord;
in vec3 exColor;
in vec4 vertPostion;
uniform sampler2D tex;
uniform float t;
uniform vec4 skyColor;
out vec4 outColor;

in float visibility;

void main(void)
{
	float timeScaled = t/500;
	vec4 lightColor1 = vec4(0.5216, 0.4784, 0.302, 1.0); //ambient light
	vec4 lightColor2 = vec4(0.4314, 0.3804, 0.2824, 1.0); //ambient light
	vec4 color = vec4(exColor, 1.0) * texture(tex, texCoord);
	float fogFactor = clamp(visibility, 0.0, 1.0);
	vec4 fragColor = mix(color, skyColor, fogFactor);

	vec4 fragColorDark1 = fragColor + lightColor1 * 0.1 * (1 - fogFactor);
	vec4 fragColorDark2 = fragColor + lightColor2 * 0.1 * (1 - fogFactor);

	vec4 outColor1 = sin(vertPostion.x + timeScaled) * fragColor + (1.0 - sin(vertPostion.x + timeScaled)) * fragColorDark1;
	vec4 outColor2 = sin(vertPostion.z + timeScaled) * fragColor + (1.0 - sin(vertPostion.z + timeScaled)) * fragColorDark2;
	outColor = outColor1 * 0.5 + outColor2 * 0.5;
}
