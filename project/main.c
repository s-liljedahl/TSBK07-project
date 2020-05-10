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
#include "camera-mov.h"
#include "terrain.h"

mat4 projectionMatrix;

void init(void)
{
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 500.0);

	dumpInfo();

	init_terrain(projectionMatrix);
	printError("init shader");
	init_ship(projectionMatrix);
	printError("init ship");
	init_grass(projectionMatrix);
	printError("init grass");
	init_skybox();
	printError("init skybox");

}

void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 total, modelView, camMatrix;

	printError("pre display");

	camMatrix = lookAtv(cameraPos, VectorAdd(cameraPos, cameraFront), cameraUp);
	modelView = T(0.0f, 5.0f, 0.0f);
	total = Mult(camMatrix, modelView);

	mat4 skybox_s = S(10.0f, 10.0f, 10.0f);
	mat4 skybox_t = T(cameraPos.x, - 3.0f, cameraPos.z);
	mat4 skybox_res = Mult(skybox_t, skybox_s);

	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	// skybox
	glDisable(GL_DEPTH_TEST);
	draw_skybox(projectionMatrix, camMatrix, skybox_res);
	glEnable(GL_DEPTH_TEST);
	printError("display skybox");

	// ship
	draw_ship(total, cameraPos, t);
	printError("display ship");
	
	// grass
	draw_grass(total, cameraPos, t);
	printError("display grass");
	
	//shark
	draw_shark(t, total);
	printError("display shark");
	
	//fish
	draw_fish(camMatrix, cameraPos, cameraFront, cameraUp);
	
	// terrain
	draw_terrain(cameraPos, total, t);
	printError("display terrain");

	glutSwapBuffers();
}

void timer(int i)
{
	glutTimerFunc(20, &timer, i);
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (600, 600);
	glutCreateWindow ("TSBK07 Project");
	glutDisplayFunc(display);
	init ();
	glutTimerFunc(20, &timer, 0);
	glutSpecialFunc(SpecialKeyHandler);
	glutPassiveMotionFunc(mouse);
	glutMainLoop();
	exit(0);
}
