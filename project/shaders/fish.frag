#version 150

in vec3 exNormal;
in vec3 exColor;
out vec4 out_Color;

void main(void)
{
	out_Color = vec4(exColor, 1.0f);
}
