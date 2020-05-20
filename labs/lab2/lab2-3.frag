#version 150

in vec3 exColor;
out vec4 out_Color;
in vec2 texcoord;
uniform sampler2D texUnit;

void main(void)
{
	out_Color = texture(texUnit, texcoord);
}
