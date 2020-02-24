#version 150

out vec4 outColor;
in vec2 texCoord;
in vec3 exColor;
uniform sampler2D tex;

void main(void)
{
	outColor = vec4(exColor, 1.0) * texture(tex, texCoord);
	// outColor = vec4(exColor, 1.0);
}
