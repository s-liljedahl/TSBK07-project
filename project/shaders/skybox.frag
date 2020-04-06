#version 150

in vec2 texCoord;
in vec3 normal;
uniform sampler2D texUnit;

out vec4 out_Color;

void main(void)
{
    out_Color = texture(texUnit, texCoord);
}
