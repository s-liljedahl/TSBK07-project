#version 150

in vec3 exColor;
out vec4 out_Color;

void main(void)
{
	// out_Color = vec4(exColor, 1.0, 0.0, 0.6667, 1.0);
	out_Color = vec4(exColor, 1.0);
}
