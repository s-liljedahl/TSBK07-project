#version 150

in vec2 texCoord;
uniform sampler2D skyTex;

out vec4 out_Color;

void main(void)
{
    out_Color = vec4(0.2, 0.6353, 0.9255, 1.0);
    // out_Color = texture(skyTex, texCoord);
}
