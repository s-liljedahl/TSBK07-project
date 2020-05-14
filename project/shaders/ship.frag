#version 150

in vec3 normal;
in vec3 exColor;
in vec3 vertPosition;

uniform float visibility;
uniform float t;
uniform vec3 cameraPos;
out vec4 outColor;

void main(void)
{
	const float kd = 0.3;
	const float kspec = 0.4;	
	const float specularExponent = 100;
	const vec3 lightSourceDir = vec3(0.5, 1.0, 0.0);
	const vec3 lightSourceColor = vec3(0.7686, 1.0, 0.8863);

	vec3 lightDirection = normalize(lightSourceDir - vertPosition);

	float timeScaled = t/500;

	vec4 baseColor = vec4(0.0902, 0.102, 0.0275, 1.0);
	vec4 skyColor = vec4(0.2, 0.6353, 0.9255, 1.0); // same as skybox
	vec4 lightColor1 = vec4(0.5216, 0.4784, 0.302, 1.0); //ambient light
	vec4 lightColor2 = vec4(0.4314, 0.3804, 0.2824, 1.0); //ambient light

	float shade = dot(normalize(normal), lightSourceDir);
	vec4 color = shade * baseColor;
	float fogFactor = clamp(visibility, 0.0, 1.0);
	vec4 fragColor = mix(color, skyColor, fogFactor);

	vec4 fragColorDark1 = fragColor + lightColor1 * 0.1 * (1 - fogFactor);
	vec4 fragColorDark2 = fragColor + lightColor2 * 0.1 * (1 - fogFactor);
	vec4 outColor1 = sin(vertPosition.x/80 + timeScaled) * fragColor + (1.0 - sin(vertPosition.x/80 + timeScaled)) * fragColorDark1;
	vec4 outColor2 = sin(vertPosition.z/80 + timeScaled) * fragColor + (1.0 - sin(vertPosition.z/80 + timeScaled)) * fragColorDark2;
  	vec4 colorRes = outColor1 * 0.5 + outColor2 * 0.5;

	// diffuse light
	float cos_theta = dot(lightDirection, normalize(vec3(colorRes)));
	vec3 i_diffuse = kd * lightSourceColor * cos_theta;

	outColor = colorRes;

}
