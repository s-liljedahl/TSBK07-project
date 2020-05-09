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
#include<math.h>

#include "skybox-loader.h"
#include "terrain.h"

#define pi 3.14

mat4 projectionMatrix;
// vertex array object
Model *grass;
// Reference to shader program
GLuint program_grass;

// controls
bool firstMouse = true;
float yaw = -90.0f;
float pitch = 90.0f;
vec3 direction;
vec3 cameraPos = {100.0f, 10.0f,  100.0f};
vec3 cameraFront = {0.0f, 0.0f, -1.0f};
vec3 cameraUp = {0.0f, 1.0f,  0.0f};
float lastX = 775.0f / 2;
float lastY = 775.0f / 2;
vec3 oct_array[5] = {}; 
float rad_oct = 5.0f;

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

void init(void)
{
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 500.0);

	dumpInfo();

	printError("init shader");
	init_terrain(projectionMatrix);
	init_skybox();
	printError("init skybox");

	grass = LoadModelPlus("resources/seahorse.obj");
	program_grass = loadShaders("shaders/grass.vert", "shaders/grass.frag");
	glUseProgram(program_grass);

}

void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 total, modelView, camMatrix;

	printError("pre display");

	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

	camMatrix = lookAtv(cameraPos, VectorAdd(cameraPos, cameraFront), cameraUp);
	modelView = T(0.0f, 5.0f, 0.0f);
	total = Mult(camMatrix, modelView);

	mat4 skybox_s = S(10.0f, 10.0f, 10.0f);
	mat4 skybox_t = T(cameraPos.x, cameraPos.y - 5.0f, cameraPos.z);
	mat4 skybox_res = Mult(skybox_t, skybox_s);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	// skybox
	glDisable(GL_DEPTH_TEST);
	draw_skybox(projectionMatrix, camMatrix, skybox_res);
	glEnable(GL_DEPTH_TEST);
	printError("display skybox");
	
	// terrain
	draw_terrain(cameraPos, total, t);
	printError("display terrain");


	printError("display 2");
	glutSwapBuffers();
}

void timer(int i)
{
	glutTimerFunc(20, &timer, i);
	glutPostRedisplay();
}

void SpecialKeyHandler(int key)
{
	const float cameraSpeed = 1.0f;
	GLfloat cam_y = getHeight(cameraPos.x, cameraPos.z);
	vec3 zero_y = {cameraPos.x,cam_y + 2.0f,cameraPos.z};

	if (key == GLUT_KEY_RIGHT)
		if (cameraPos.y >= cam_y + 2) {
    		cameraPos = VectorAdd(cameraPos, ScalarMult(Normalize(CrossProduct(cameraFront, cameraUp)), cameraSpeed));
		}
		else {
			cameraPos = VectorAdd(zero_y, ScalarMult(Normalize(CrossProduct(cameraFront, cameraUp)), cameraSpeed));
		}
	else if (key == GLUT_KEY_LEFT)
		if (cameraPos.y >= cam_y + 2) {
			cameraPos = VectorSub(cameraPos, ScalarMult(Normalize(CrossProduct(cameraFront, cameraUp)), cameraSpeed));
		}
		else {
			cameraPos = VectorSub(zero_y, ScalarMult(Normalize(CrossProduct(cameraFront, cameraUp)), cameraSpeed));
		}
	else if (key == GLUT_KEY_UP)
	{
		if (cameraPos.y >= cam_y + 2) {
			cameraPos = VectorAdd(cameraPos, ScalarMult(cameraFront, cameraSpeed));
		}
		else {
			cameraPos = VectorAdd(zero_y, ScalarMult(cameraFront, cameraSpeed));
		}
	}
	else if (key == GLUT_KEY_DOWN)
	{
		if (cameraPos.y >= cam_y + 2) {
			cameraPos = VectorSub(cameraPos, ScalarMult(cameraFront, cameraSpeed));
		}
		else {
			cameraPos = VectorSub(zero_y, ScalarMult(cameraFront, cameraSpeed));
		}
	}
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (600, 600);
	glutCreateWindow ("TSBK07 Lab 4");
	glutDisplayFunc(display);
	init ();
	glutTimerFunc(20, &timer, 0);
	glutSpecialFunc(SpecialKeyHandler);
	glutPassiveMotionFunc(mouse);
	glutMainLoop();
	exit(0);
}
