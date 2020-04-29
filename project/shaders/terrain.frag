#version 150

out vec4 outColor;
in vec2 texCoord;
in vec3 exColor;
in vec4 vertPostion;
uniform sampler2D tex;
uniform sampler2D dirt;

in float visibility;

void main(void)
{
	vec4 color;
	if (vertPostion.y < 1) {
		color = vec4(exColor, 1.0) * texture(dirt, texCoord);
	}
	else if (vertPostion.y >= 1 && vertPostion.y < 5) {
		float f = sin(vertPostion.y / 5);
		color = vec4(exColor, 1.0) * ( ( (1 - f) * texture(dirt, texCoord)) + (f *  texture(tex, texCoord)) );
	} else {
		color = vec4(exColor, 1.0) * texture(tex, texCoord);
	}

	float fogFactor = clamp(visibility, 0.0, 1.0);
	vec4 skyColor = vec4(0.1, 0.5, 0.6, 1.0);
	vec4 fragColor = mix(color, skyColor, fogFactor);
	outColor = fragColor;
}
