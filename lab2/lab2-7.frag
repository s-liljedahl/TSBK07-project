#version 150

in vec3 exColor;
out vec4 out_Color;
in vec2 texcoord;
uniform sampler2D texUnit;

void main(void)
{
	out_Color = vec4(exColor,1.0) * texture(texUnit, texcoord);
}
