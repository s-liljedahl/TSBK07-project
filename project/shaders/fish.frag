#version 150

in vec3 exNormal;
in vec3 exColor;
out vec4 out_Color;

void main(void)
{
	vec4 color = vec4(0.9333, 0.3725, exColor.y, 1.0);
 	out_Color = color;
}
