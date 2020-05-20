#version 150

in vec3 exColor;
out vec4 out_Color;
in vec2 texcoord;

void main(void)
{
	out_Color = vec4(texcoord, texcoord);
}
