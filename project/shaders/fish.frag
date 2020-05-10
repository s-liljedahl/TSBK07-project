#version 150

in vec3 exNormal;
out vec4 out_Color;

void main(void)
{
	out_Color = vec4(exNormal.x, exNormal.y, 0.4f, 1.0f);
}
