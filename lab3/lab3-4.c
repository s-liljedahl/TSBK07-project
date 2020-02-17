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
#define far 70.0
#define right 5.0
#define left -5.0
#define top 5.0
#define bottom -5.0

// Globals
// Data would normally be read from files

// Reference to shader program
GLuint program, program_sky, program_grass;
GLuint myTex, myTex1;

float rotate_x = 2.0f;
float rotate_z = -20.0f;
float rotate_y = 1.0f;

unsigned int groundArrayObjID;
unsigned int groundBufferObjID;


vec3 lightSourcesColorsArr[] = 
{ 
	{1.0f, 0.0f, 0.0f}, // Red light

	{0.0f, 1.0f, 0.0f}, // Green light

	{0.0f, 0.0f, 1.0f}, // Blue light

	{1.0f, 1.0f, 1.0f} 
}; // White light

GLint isDirectional[] = {0,0,1,1};

vec3 lightSourcesDirectionsPositions[] = 
{ 
	{10.0f, 5.0f, 0.0f}, // Red light, positional

	{0.0f, 5.0f, 10.0f}, // Green light, positional

	{-1.0f, 0.0f, 0.0f}, // Blue light along X

	{0.0f, 0.0f, -1.0f} 
}; // White light along Z

GLfloat specularExponent[] = {100.0, 200.0, 60.0, 50.0, 300.0, 150.0};

GLfloat projectionMatrix[] =
{
    2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,

    0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,

    0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),

    0.0f, 0.0f, -1.0f, 0.0f
};

GLfloat groundMatrix[] =
{
    // Bottom 1
    -80.0f, -0.0f,  80.0f,
    80.0f, -0.0f,  80.0f,
    80.0f, -0.0f, -80.0f,

    // Bottom 2
    -80.0f, -0.0f,  80.0f,
    -80.0f, -0.0f, -80.0f,
    80.0f, -0.0f, -80.0f,
};

void OnTimer(int value)
{
  glutPostRedisplay();
  glutTimerFunc(20, &OnTimer, value);
}

// vertex array object
Model *blade;
Model *mill;
Model *roof;
Model *balcony;
Model *skybox;

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

void KeyHandler(int key, int x, int y) 
{
	if (key == 'w')
    	rotate_y += 1;
	else if (key == 's')
		rotate_y -= 1;
}

void init(void)
{
	printError("GL inits");

	blade = LoadModelPlus("blade.obj");
	mill = LoadModelPlus("windmill-walls.obj");
	roof = LoadModelPlus("windmill-roof.obj");
	balcony = LoadModelPlus("windmill-balcony.obj");
    skybox = LoadModelPlus("skybox.obj");

	dumpInfo();

	// GL inits
	glClearColor(1.0,1.0,1.0,0); //bakgrundsfärg

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);

	// Load and compile shader
	program_sky = loadShaders("lab3-3.vert", "lab3-3sky.frag");
	LoadTGATextureSimple("SkyBox512.tga", &myTex);

	program_grass = loadShaders("lab3-3.vert", "lab3-3grass.frag");
	LoadTGATextureSimple("maskros512.tga", &myTex1);

	program = loadShaders("lab3-3.vert", "lab3-3.frag");

    printError("init shader");

	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &groundArrayObjID);
	glBindVertexArray(groundArrayObjID);
	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &groundBufferObjID);

	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, groundBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 2*9*sizeof(GLfloat), groundMatrix, GL_STATIC_DRAW);

	glVertexAttribPointer(glGetAttribLocation(program_grass, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program_grass, "in_Position"));

    // Bind COLOR in variable
	glVertexAttribPointer(glGetAttribLocation(program_grass, "inNormal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	// Array COLOR is active
	glEnableVertexAttribArray(glGetAttribLocation(program_grass, "inNormal"));

	glActiveTexture(GL_TEXTURE0);
	//glActiveTexture(GL_TEXTURE1);

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


	float scale_x = 10.0f;
	float scale_y = 20.0f;
	float scale_z = 10.0f;

	mat4 skybox_scale = S(scale_x, scale_y, scale_z);
	mat4 skybox_trans = T(rotate_x, 1.0f, rotate_z);
	mat4 skybox_res = Mult(skybox_trans, skybox_scale);

	const float radius = 20.0f;
	float camx = sin(t / 1000) * radius;
	float camz = cos(t / 1000) * radius;

	view = lookAt(rotate_x, rotate_y, rotate_z,
					0.0f,0.0f,0.0f,
					0.0f,1.0f,0.0f);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(program_sky);

	glUniformMatrix4fv(glGetUniformLocation(program_sky, "view"), 1, GL_TRUE, view.m);
	glUniformMatrix4fv(glGetUniformLocation(program_sky, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program_sky, "myMatrix"), 1, GL_TRUE, transMill.m);

	glBindTexture(GL_TEXTURE_2D, myTex); //sky texture	
	glDisable(GL_DEPTH_TEST);
	glUniformMatrix4fv(glGetUniformLocation(program_sky, "myMatrix"), 1, GL_TRUE, skybox_res.m);
	DrawModel(skybox, program_sky, "in_Position", "inNormal", "inTexCoord");
	glEnable(GL_DEPTH_TEST);

	glUseProgram(program_grass);

	glUniformMatrix4fv(glGetUniformLocation(program_grass, "view"), 1, GL_TRUE, view.m);
	glUniformMatrix4fv(glGetUniformLocation(program_grass, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program_grass, "myMatrix"), 1, GL_TRUE, transMill.m);	

	//glUniformMatrix4fv(glGetUniformLocation(program_grass, "myMatrix"), 1, GL_TRUE, T(0,-5,0).m);
	glBindTexture(GL_TEXTURE_2D, myTex1); //grass text
	glBindVertexArray(groundArrayObjID);	// Select VAO
	glDrawArrays(GL_TRIANGLES, 0, 9*2);	//

	glUseProgram(program);

	glUniform1i(glGetUniformLocation(program, "texUnit"), 0);
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, view.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, transMill.m);

	DrawModel(mill, program, "in_Position", "inNormal", "");

	glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, blade1_res.m);
	DrawModel(blade, program, "in_Position", "inNormal", "");

	glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, blade2_res.m);
	DrawModel(blade, program, "in_Position", "inNormal", "");

	glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, blade3_res.m);
	DrawModel(blade, program, "in_Position", "inNormal", "");

	glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, blade4_res.m);
	DrawModel(blade, program, "in_Position", "inNormal", "");

	glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, transRoof.m);
	DrawModel(roof, program, "in_Position", "inNormal", "");

	glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, bal_res.m);
	DrawModel(balcony, program, "in_Position", "inNormal", "");

	glUniform3fv(glGetUniformLocation(program, "lightSourcesDirPosArr"), 4, &lightSourcesDirectionsPositions[0].x);
	glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), 4, &lightSourcesColorsArr[0].x);
	glUniform1f(glGetUniformLocation(program, "specularExponent"), specularExponent[i]);
	glUniform1iv(glGetUniformLocation(program, "isDirectional"), 4, isDirectional);


//	glUniform1i(glGetUniformLocation(program, "texUnit"), 0);

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
	glutKeyboardFunc(KeyHandler);
	glutMainLoop();
	return 0;
}
