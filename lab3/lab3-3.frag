#version 150

in vec3 exColor;
out vec4 out_Color;
in vec2 texcoord;
uniform sampler2D texUnit;
uniform bool useText;

void main(void)
{
	if (useText) 
	{
		out_Color = texture(texUnit, texcoord);
	}
	else 
	{
		out_Color = vec4(exColor, 1.0);
	}
}
