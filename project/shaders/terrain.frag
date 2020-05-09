#version 150

out vec4 outColor;
in vec2 texCoord;
in vec3 exColor;
in vec4 vertPostion;
uniform sampler2D tex;
uniform float t;

in float visibility;

void main(void)
{
	vec4 skyColor = vec4(0.2, 0.6353, 0.9255, 1.0); // same as skybox
	vec4 lightColor = vec4(0.2784, 0.2549, 0.2039, 0.103); //ambient light
	vec4 color = vec4(exColor, 1.0) * texture(tex, texCoord);
	float fogFactor = clamp(visibility, 0.0, 1.0);
	vec4 fragColor = mix(color, skyColor, fogFactor);
	vec4 fragColorDark = fragColor + lightColor * 0.05;
	outColor = sin(vertPostion.x + t / 500) * fragColor + (1.0 - sin(vertPostion.x + t / 500)) * fragColorDark;
}
