#version 150

out vec4 out_Color;
in vec3 exColor;

in vec4 gl_FragCoord;

void main(void)
{
	if(gl_FragCoord.x < gl_FragCoord.y)
        out_Color = vec4(1.0, 0.0, 0.6667, 1.0);
    else
        out_Color = vec4(1.0, 0.5686, 0.0, 1.0);   
}
