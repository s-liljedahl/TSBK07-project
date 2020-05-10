#ifdef __APPLE__
#include <OpenGL/gl3.h>
// Linking hint for Lightweight IDE
// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"

#include "camera-mov.h"
#include "terrain.h"

#define pi 3.14

// controls
bool firstMouse = true;
float yaw = -90.0f;
float pitch = 90.0f;

float lastX = 775.0f / 2;
float lastY = 775.0f / 2;

// // vec3 direction;
vec3 cameraFront = {0.0f, 0.0f, -1.0f};
vec3 cameraUp = {0.0f, 1.0f,  0.0f};
vec3 cameraPos = {90.0f, 7.5f,  90.0f};
vec3 direction;

TextureData ttex;
Model *tm;

float radians(float degree) {
	float rad = (pi/180) * degree;
	return rad;
}

void mouse(int xpos, int ypos)
{
	if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
	lastX = xpos;
	lastY = ypos;

	const float sensitivity = 2.0f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if(pitch > 89.0f)
		pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

	direction.x = cos(radians(yaw)) * cos(radians(pitch));
    direction.y = sin(radians(pitch));
    direction.z = sin(radians(yaw)) * cos(radians(pitch));
    cameraFront = Normalize(direction);
}

void SpecialKeyHandler(int key)
{
	const float cameraSpeed = 1.0f;
	const float height_y = 7.5f;
	GLfloat cam_y = getHeight(cameraPos.x + cameraFront.x, cameraPos.z + cameraFront.z);
	vec3 zero_y = {cameraPos.x+ cameraFront.x,cam_y + height_y, cameraPos.z+ cameraFront.z};

	if (key == GLUT_KEY_RIGHT)
		if (cameraPos.y >= cam_y + height_y) {
    		cameraPos = VectorAdd(cameraPos, ScalarMult(Normalize(CrossProduct(cameraFront, cameraUp)), cameraSpeed));
		}
		else {
			cameraPos = VectorAdd(zero_y, ScalarMult(Normalize(CrossProduct(cameraFront, cameraUp)), cameraSpeed));
		}
	else if (key == GLUT_KEY_LEFT)
		if (cameraPos.y >= cam_y + height_y) {
			cameraPos = VectorSub(cameraPos, ScalarMult(Normalize(CrossProduct(cameraFront, cameraUp)), cameraSpeed));
		}
		else {
			cameraPos = VectorSub(zero_y, ScalarMult(Normalize(CrossProduct(cameraFront, cameraUp)), cameraSpeed));
		}
	else if (key == GLUT_KEY_UP)
	{
		if (cameraPos.y >= cam_y + height_y) {
			cameraPos = VectorAdd(cameraPos, ScalarMult(cameraFront, cameraSpeed));
		}
		else {
			cameraPos = VectorAdd(zero_y, ScalarMult(cameraFront, cameraSpeed));
		}
	}
	else if (key == GLUT_KEY_DOWN)
	{
		if (cameraPos.y >= cam_y + height_y) {
			cameraPos = VectorSub(cameraPos, ScalarMult(cameraFront, cameraSpeed));
		}
		else {
			cameraPos = VectorSub(zero_y, ScalarMult(cameraFront, cameraSpeed));
		}
	}
}