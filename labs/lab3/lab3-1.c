// Lab 1-4.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.

// Should work as is on Linux and Mac. MS Windows needs GLEW or glee.
// See separate Visual Studio version of my demos.
#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include <math.h>
#include "LoadTGA.h"

#define near 5.0
#define far 40.0
#define right 5.0
#define left -5.0
#define top 5.0
#define bottom -5.0

// Globals
// Data would normally be read from files

// Reference to shader program
GLuint program;
GLuint myTex;

float rotate_x = 2;
float rotate_z = -5;

GLfloat projectionMatrix[] =
{
    2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,

    0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,

    0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),

    0.0f, 0.0f, -1.0f, 0.0f
};

void OnTimer(int value)
{
  glutPostRedisplay();
  glutTimerFunc(20, &OnTimer, value);
}

// vertex array object
unsigned int bunnyVertexArrayObjID;
Model *blade;
Model *mill;
Model *roof;
Model *balcony;

void SpecialKeyHandler(int key, int x, int y) 
{
	if (key == GLUT_KEY_RIGHT)
    	rotate_z += 1;
	else if (key == GLUT_KEY_LEFT)
		rotate_z -= 1;
	else if (key == GLUT_KEY_UP)
		rotate_x += 1;
	else if (key == GLUT_KEY_DOWN)
		rotate_x -= 1;
}

void init(void)
{
	printError("GL inits");

	blade = LoadModelPlus("blade.obj");
	mill = LoadModelPlus("windmill-walls.obj");
	roof = LoadModelPlus("windmill-roof.obj");
	balcony = LoadModelPlus("windmill-balcony.obj");

	dumpInfo();

	// GL inits
	glClearColor(1.0,1.0,1.0,0); //bakgrundsfärg

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);

	// Load and compile shader
	program = loadShaders("lab3-1.vert", "lab3-1.frag");

	printError("init arrays");
}


void display(void)
{
	printError("pre display");
    mat4 rot, trans, total, transMill, view, transRoof, transBal;

	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

	GLfloat ticker = (t / 1000);
	GLfloat tickerSin = sin(t / 1000);
	GLfloat tickerCos = cos(t / 1000);

	mat4 ry_init = Ry(3.14 / 2); // vrid bladet ett halft varv runt y, bladen är plan mot vyn

	mat4 rz = Rz(ticker); // tids rotationen


	// fördela bladen
	mat4 rz2_init = Rz(3.14 / 2); // flytta 90 grader
	mat4 rz3_init = Rz(3.14); // flytta 180 grader
	mat4 rz4_init = Rz(- 3.14 / 2); // flytta minus -90
	mat4 ry4_init = Ry(3.14*2); 

  	trans = T(0, 4.5, -4);  // flytta bladen
	transMill = T(0, -5, 0); // flytta huset
	transRoof = T(0, -4, 0); //flytta tak
	transBal = T(0, -5, 0); //flytta balkong

	// initiera blade position på kvarnen
	mat4 blade2Init = Mult(rz2_init, ry_init);
	mat4 blade3Init = Mult(rz3_init, ry_init);
	mat4 blade4Init = Mult(rz4_init, ry_init);

	// initiera trappan på kvarnen
	mat4 balInit = Mult(ry4_init, ry_init);

	// lägg på tidsroationen
	mat4 bladeRot1 = Mult(rz, ry_init);
	mat4 bladeRot2 = Mult(rz, blade2Init);
	mat4 bladeRot3 = Mult(rz, blade3Init);
	mat4 bladeRot4 = Mult(rz, blade4Init);

	// lägg på transformationen
	mat4 blade1_res = Mult(trans, bladeRot1);
	mat4 blade2_res = Mult(trans, bladeRot2);
	mat4 blade3_res = Mult(trans, bladeRot3);
	mat4 blade4_res = Mult(trans, bladeRot4);
	mat4 bal_res = Mult(transBal, balInit);

	const float radius = 20.0f;
	float camx = sin(t / 1000) * radius;
	float camz = cos(t / 1000) * radius;

	view = lookAt(camx, 0.0f, camz,
					0.0f,0.0f,0.0f,
					0.0f,1.0f,0.0f);


	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawModel(mill, program, "in_Position", "inNormal", "inTexCoord");

	glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, blade1_res.m);
	DrawModel(blade, program, "in_Position", "inNormal", "inTexCoord");

	glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, blade2_res.m);
	DrawModel(blade, program, "in_Position", "inNormal", "inTexCoord");

	glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, blade3_res.m);
	DrawModel(blade, program, "in_Position", "inNormal", "inTexCoord");

	glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, blade4_res.m);
	DrawModel(blade, program, "in_Position", "inNormal", "inTexCoord");

	glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, transRoof.m);
	DrawModel(roof, program, "in_Position", "inNormal", "inTexCoord");

	glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, bal_res.m);
	DrawModel(balcony, program, "in_Position", "inNormal", "inTexCoord");

	glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, transMill.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, view.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);

	printError("display");
	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("GL3 white triangle example");
	glutDisplayFunc(display);
	init ();
	glutTimerFunc(20, &OnTimer, 0);
	glutSpecialFunc(SpecialKeyHandler);
	glutMainLoop();
	return 0;
}
