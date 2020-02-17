#version 150

in vec3 exColor;
in vec3 frag_position;
out vec4 out_Color;

uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform float specularExponent[4];
uniform bool isDirectional[4];

uniform vec3 viewDirection;

uniform mat4 view;


void main(void)
{
	vec3 i_diffuse;
	vec3 i_ambient;
	vec3 i_spec;
	vec3 result;
	vec3 total;

	// vec3 viewDirection = vec3(view[0][0], view[0][1], view[0][2]);

	const float kd = 0.1;
	const float kspec = 0.4;

	// ambient light
	const vec3 lightColor = vec3(0.5, 0.9, 1.0); //ambient light
	float ambientStrength = 0.1; // ljusstyrka
	i_ambient = ambientStrength * lightColor;
	total = i_ambient;

	for(int i = 0; i < 4; i++)
	{

		vec3 direction;

		if (isDirectional[i]) {
			direction = normalize(lightSourcesDirPosArr[i]);
		} else {
			direction = normalize(lightSourcesDirPosArr[i] - frag_position);
		}


		// diffuse light
		float cos_theta = dot(direction, normalize(exColor));
		i_diffuse = kd *  lightSourcesColorArr[i] * cos_theta;

		// specular
		vec3 r = (2 * normalize(exColor) * cos_theta) - direction;

		float cos_phi = max(0, dot(r, normalize(viewDirection - frag_position)));

		i_spec =  kspec * lightSourcesColorArr[i] * pow(cos_phi, specularExponent[i]);

		// resulting light
		result = i_spec + i_diffuse;
		total += result;
	};

	out_Color = vec4(total, 1.0);
	// out_Color = vec4(exColor, 1.0);
}
