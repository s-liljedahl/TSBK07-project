#version 150

in vec3 exNormal;
out vec4 out_Color;
uniform bool hasCollision;

void main(void)
{
	if (hasCollision) {
		out_Color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	} else {
		out_Color = vec4(0.4f, exNormal.y, exNormal.z, 1.0f);
	}

}
