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
#include <math.h>

// Reference to shader program
GLuint program;

GLfloat myMatrix[] =
{
  0.7f, -0.7f, 0.0f, 0.0f,
  0.7f, 0.7f, 0.0f, 0.0f,
  0.0f, 0.0f, 1.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 1.0f
};

// Globals
// Data would normally be read from files
GLfloat vertices[] =
{
	-0.5f,-0.5f,0.0f,
	-0.5f,0.5f,0.0f,
	0.5f,-0.5f,0.0f
};

// // Globals
// // Data would normally be read from files
// GLfloat colors[] =
// {
// 	-0.5f,-0.5f,0.0f,
// 	-0.5f,0.5f,0.0f,
// 	0.5f,-0.5f,0.0f
// };


// Globals
// Data would normally be read from files
GLfloat colors[] =
{
	0.7f, 0.0f,0.7f,
	0.0f,1.0f,0.0f,
	0.58f,0.58f,0.58f
};


// vertex array object
unsigned int vertexArrayObjID;

void init(void)
{
	// vertex buffer object, used for uploading the geometry
	unsigned int vertexBufferObjID;
	unsigned int colorBufferObjID;

	dumpInfo();

	// GL inits
	glClearColor(1.0,0.2,0.5,0); //bakgrundsfärg

	glDisable(GL_DEPTH_TEST);

	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab1-4.vert", "lab1-4.frag");
	printError("init shader");

	// Upload geometry to the GPU:

	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);

	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &vertexBufferObjID);
	glGenBuffers(1, &colorBufferObjID);

	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	// Bind in variable
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	// Array is active
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

	// VBO for COLOR data
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), colors, GL_STATIC_DRAW);

	// Bind COLOR in variable
	glVertexAttribPointer(glGetAttribLocation(program, "inNormal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	// Array COLOR is active
	glEnableVertexAttribArray(glGetAttribLocation(program, "inNormal"));

	// End of upload of geometry

	printError("init arrays");
}


void display(void)
{

	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(vertexArrayObjID);	// Select VAO

	glDrawArrays(GL_TRIANGLES, 0, 3);	// draw object
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

	glutMainLoop();
	return 0;
}
