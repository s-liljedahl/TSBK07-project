// Lab 1-1.
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

// vertex array object
unsigned int vertexArrayObjID;

<<<<<<< Updated upstream

void OnTimer(int value)
{
  glutPostRedisplay();
=======
void OnTimer(int value)
{
  glutPostRedisplay();

>>>>>>> Stashed changes
  glutTimerFunc(20, &OnTimer, value);
}

void init(void)
{
	// vertex buffer object, used for uploading the geometry
	unsigned int vertexBufferObjID;
	// Reference to shader program
	GLuint program;

	dumpInfo();

	// GL inits
	glClearColor(1.0,0.2,0.5,0); //bakgrundsfärg
	glDisable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab1-3.vert", "lab1-3.frag");
	printError("init shader");



	// Upload geometry to the GPU:

	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);
	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &vertexBufferObjID);

	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));
  glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, myMatrix);
	// End of upload of geometry

	printError("init arrays");
}

void display(void)
{
	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

	printError("pre display");
  GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

	int a = t/500 * 3.14;
	GLfloat rotationMatrix[] =
	{
	  cos(a), -sin(a), 0.0f, 0.0f,
	  sin(a), cos(a), 0.0f, 0.0f,
	  0.0f, 0.0f, 1.0f, 0.0f,
	  0.0f, 0.0f, 0.0f, 1.0f
	};

	// GLfloat rotationMatrix[] =
	// {
	//   0.7f*t, -0.7f, 0.0f, 0.0f,
	//   0.7f, 0.7f, 0.0f, 0.0f,
	//   0.0f, 0.0f, 1.0f, 0.0f,
	//   0.0f, 0.0f, 0.0f, 1.0f
	// };

	// Reference to shader program
	// GLuint program;
	// program = loadShaders("lab1-3.vert", "lab1-3.frag");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(vertexArrayObjID);	// Select VAO
	glDrawArrays(GL_TRIANGLES, 0, 3);	// draw object
	glUniformMatrix4fv(glGetAttribLocation(program, "myMatrix"), 1, GL_TRUE, rotationMatrix);
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
<<<<<<< Updated upstream

	// Re-run every 20 milliseconds
	glutTimerFunc(20, &OnTimer, 0);

=======
  glutTimerFunc(20, &OnTimer, 0);
>>>>>>> Stashed changes
	glutMainLoop();
	return 0;
}
