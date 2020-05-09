#version 150

in vec2 texCoord;
uniform sampler2D skyTex;

out vec4 out_Color;

void main(void)
{
    out_Color = texture(skyTex, texCoord);
}
