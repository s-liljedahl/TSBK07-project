#version 150

in vec3 normal;
in vec3 exColor;
in vec4 vertPostion;

uniform float visibility;
uniform float t;
out vec4 outColor;

void main(void)
{
	float timeScaled = t/500;

  vec4 baseColor = vec4(0.1451, 0.1451, 0.1451, 1.0);
  vec4 skyColor = vec4(0.2, 0.6353, 0.9255, 1.0); // same as skybox
	
  vec4 lightColor1 = vec4(0.5216, 0.4784, 0.302, 1.0); //ambient light
	vec4 lightColor2 = vec4(0.4314, 0.3804, 0.2824, 1.0); //ambient light

	vec4 color = vec4(exColor, 1.0) * baseColor;
  float fogFactor = clamp(visibility, 0.0, 1.0);
	vec4 fragColor = mix(color, skyColor, fogFactor);

	vec4 fragColorDark1 = fragColor + lightColor1 * 0.1 * (1 - fogFactor);
	vec4 fragColorDark2 = fragColor + lightColor2 * 0.1 * (1 - fogFactor);
	vec4 outColor1 = sin(vertPostion.x/100 + timeScaled) * fragColor + (1.0 - sin(vertPostion.x/100 + timeScaled)) * fragColorDark1;
	vec4 outColor2 = sin(vertPostion.z/100 + timeScaled) * fragColor + (1.0 - sin(vertPostion.z/100 + timeScaled)) * fragColorDark2;
	
  outColor = outColor1 * 0.5 + outColor2 * 0.5;
}
