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

#define near 1.0
#define far 30.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5

// Globals
// Data would normally be read from files

// Reference to shader program
GLuint program;
GLuint myTex;

GLfloat myMatrix[] =
{
  0.7f, -0.7f, 0.0f, 0.0f,
  0.7f, 0.7f, 0.0f, 0.0f,
  0.0f, 0.0f, 1.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 1.0f
};

GLfloat vertices[] =
{
		// front
		0.0f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,

		// Right
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, -0.5f,

		// Back
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		// Left
		0.0f, 0.5f, 0.0f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f, 0.5f,

		// Bottom 1
		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f, -0.5f,

		// Bottom 2
		-0.5f, -0.5f,  0.5f,
		 -0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,

};


GLfloat colors[] =
{
	0.7f, 0.0f,0.7f,
	0.0f,1.0f,0.0f,
	0.58f,0.58f,0.58f,

	0.7f, 0.0f,0.7f,
	0.0f,1.0f,0.0f,
	0.58f,0.58f,0.58f,

	0.7f, 0.0f,0.7f,
	0.0f,1.0f,0.0f,
	0.58f,0.58f,0.58f,

	0.0f, 1.0f, 0.1f,
	1.0f, 0.5f, 0.1f,
	0.0f, 0.0f, 0.1f,

	0.4f, 0.0f,0.7f,
	0.0f, 0.5f,0.0f,
	0.58f,0.58f,0.58f,

	0.1f, 0.2f,0.7f,
	0.0f,1.0f,0.5f,
	0.58f,0.0f,0.58f,

};

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
Model *m;

void init(void)
{
	printError("GL inits");

	m = LoadModel("bunnyplus.obj");

	unsigned int bunnyTexCoordBufferObjID;
	unsigned int bunnyVertexBufferObjID;
	unsigned int bunnyIndexBufferObjID;
	unsigned int bunnyNormalBufferObjID;

	// vertex buffer object, used for uploading the geometry
	// unsigned int vertexBufferObjID;
	unsigned int colorBufferObjID;

	dumpInfo();

	// GL inits
	glClearColor(1.0,0.2,0.5,0); //bakgrundsfärg

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);

	// Load and compile shader
	program = loadShaders("lab2-4.vert", "lab2-4.frag");
    LoadTGATextureSimple("maskros512.tga", &myTex);
	printError("init shader");

	// Upload geometry to the GPU:
	glGenVertexArrays(1, &bunnyVertexArrayObjID);
	glBindVertexArray(bunnyVertexArrayObjID);

	glGenBuffers(1, &bunnyTexCoordBufferObjID);


	glGenBuffers(1, &bunnyVertexBufferObjID);
	glGenBuffers(1, &bunnyIndexBufferObjID);
	glGenBuffers(1, &bunnyNormalBufferObjID);

	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, bunnyVertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->vertexArray, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

	// VBO for normal data
	glBindBuffer(GL_ARRAY_BUFFER, bunnyNormalBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->normalArray, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "inNormal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "inNormal"));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bunnyIndexBufferObjID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->numIndices*sizeof(GLuint), m->indexArray, GL_STATIC_DRAW);

	// End of upload of geometry

	if (m->texCoordArray != NULL)
    {
        glBindBuffer(GL_ARRAY_BUFFER, bunnyTexCoordBufferObjID);
        glBufferData(GL_ARRAY_BUFFER, m->numVertices*2*sizeof(GLfloat), m->texCoordArray, GL_STATIC_DRAW);
        glVertexAttribPointer(glGetAttribLocation(program, "inTexCoord"), 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(glGetAttribLocation(program, "inTexCoord"));
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, myTex);

	printError("init arrays");
}


void display(void)
{
	printError("pre display");
    mat4 rot, trans, total, view;

	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

	GLfloat a = (t / 1000);
	GLfloat b = cos(t / 2000);
	mat4 ry = Ry(a);
	mat4 rx = Rx(b);
	mat4 res = Mult(rx, ry);

    trans = T(0, 0, -3);

    total = Mult(trans, res);

    view = lookAt(2.0f,2.0f,2.0f,
                  0.0f,0.0f,-3.0f,
                  0.0f,1.0f,0.0f);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glBindVertexArray(bunnyVertexArrayObjID);    // Select VAO
	glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);

    glUniform1i(glGetUniformLocation(program, "texUnit"), 0);
    glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, total.m);
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
	glutMainLoop();
	return 0;
}
