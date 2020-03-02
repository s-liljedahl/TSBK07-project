#version 150

out vec4 outColor;
in vec2 texCoord;
in vec3 exColor;
in vec4 vertPostion;
uniform sampler2D tex;
uniform sampler2D dirt;

void main(void)
{
	if (vertPostion.y < 1) {
		outColor = vec4(exColor, 1.0) * texture(dirt, texCoord);
	}
	else if (vertPostion.y >= 1 && vertPostion.y < 5) {
		float f = sin(vertPostion.y / 5);
		outColor = vec4(exColor, 1.0) * ( ( (1 - f) * texture(dirt, texCoord)) + (f *  texture(tex, texCoord)) );
	} else {
		outColor = vec4(exColor, 1.0) * texture(tex, texCoord);
	}
}
