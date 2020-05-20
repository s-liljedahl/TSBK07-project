// Lab 4, terrain generation

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

mat4 projectionMatrix;
float cam_pos_x = 2.0f;
float cam_pos_y = 5.0f;
float cam_pos_z = 8.0f;

float cam_look_x = 2.0f;
float cam_look_z = 2.0f;
float cam_look_angle = 0.0f;

int mouse_x_old = 0;
int mouse_y_old = 0;

Model* GenerateTerrain(TextureData *tex)
{
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	int x, z;

	printf("count :%d\n", triangleCount);

	GLfloat *vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);

	printf("bpp %d\n", tex->bpp);
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width)*3 + 0] = 0.5* x / 1.0;
			vertexArray[(x + z * tex->width)*3 + 1] = 0.5* tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / 100.0;
			vertexArray[(x + z * tex->width)*3 + 2] = 0.5* z / 1.0;
// Normal vectors. You need to calculate these.
			normalArray[(x + z * tex->width)*3 + 0] = 0.0;
			normalArray[(x + z * tex->width)*3 + 1] = 1.0;
			normalArray[(x + z * tex->width)*3 + 2] = 0.0;
// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)*2 + 0] = x; // (float)x / tex->width;
			texCoordArray[(x + z * tex->width)*2 + 1] = z; // (float)z / tex->height;
		}
	for (x = 0; x < tex->width-1; x++)
		for (z = 0; z < tex->height-1; z++)
		{
		// Triangle 1
			indexArray[(x + z * (tex->width-1))*6 + 0] = x + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 1] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 2] = x+1 + z * tex->width;
		// Triangle 2
			indexArray[(x + z * (tex->width-1))*6 + 3] = x+1 + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 4] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 5] = x+1 + (z+1) * tex->width;
		}

	// End of terrain generation

	// Create Model and upload to GPU:

	Model* model = LoadDataToModel(
			vertexArray,
			normalArray,
			texCoordArray,
			NULL,
			indexArray,
			vertexCount,
			triangleCount*3);

	return model;
}


// vertex array object
Model *m, *m2, *tm;
// Reference to shader program
GLuint program;
GLuint tex1, tex2;
TextureData ttex; // terrain

void init(void)
{
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 50.0);

	// Load and compile shader
	program = loadShaders("terrain.vert", "terrain.frag");
	glUseProgram(program);
	printError("init shader");

	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	LoadTGATextureSimple("maskros512.tga", &tex1);

// Load terrain data

	LoadTGATextureData("fft-terrain.tga", &ttex);
	tm = GenerateTerrain(&ttex);
	printError("init terrain");
}

void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 total, modelView, camMatrix;

	printError("pre display");

	glUseProgram(program);

	// Build matrix

	vec3 cam = {cam_pos_x, cam_pos_y, cam_pos_z};
	vec3 lookAtPoint = {cam_look_x, 0, cam_look_z};

	camMatrix = lookAt(cam.x, cam.y, cam.z,
				lookAtPoint.x, lookAtPoint.y, lookAtPoint.z,
				0.0, 1.0, 0.0);
	modelView = IdentityMatrix();
	total = Mult(camMatrix, modelView);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);

	glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1
	DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");

	printError("display 2");

	glutSwapBuffers();
}

void timer(int i)
{
	glutTimerFunc(20, &timer, i);
	glutPostRedisplay();
}

void mouse(int x, int y)
{
	printf("%d %d\n", x, y);

	float CameraMoveSpeed = 0.01; 
	int deltaX;
	int deltaY;

	if ((x < 600 && y < 600) && (x > 0 && y > 0)) {
		deltaX = x - mouse_x_old;
		deltaY = y - mouse_y_old;
	} else {
		deltaX = (x < 0 ? 5 : -5);
		deltaY = (y < 0 ? 5 : -5);
	}
		
	// cam_pos_x += deltaY * cam_look_x * CameraMoveSpeed;
	// cam_pos_z += deltaY * cam_look_z * CameraMoveSpeed;

	// float rightX = -cam_look_z;
	// float rightZ = cam_look_x;

	// cam_pos_x += deltaX * rightX * CameraMoveSpeed;
	// cam_pos_z += deltaY * rightZ * CameraMoveSpeed;

	cam_pos_x += deltaX * CameraMoveSpeed;
	cam_pos_z += deltaY * CameraMoveSpeed;

	// save old mouse coordinates
	mouse_x_old = x;
	mouse_y_old = y;
}

void SpecialKeyHandler(int key)
{
	if (key == GLUT_KEY_RIGHT)
    	cam_pos_z += 1;
	else if (key == GLUT_KEY_LEFT)
		cam_pos_z -= 1;
	else if (key == GLUT_KEY_UP)
		cam_pos_x += 1;
	else if (key == GLUT_KEY_DOWN)
		cam_pos_x -= 1;
}

void KeyHandler(int key, int x, int y)
{
	// move up/down
	if (key == 'w')
    	cam_pos_y += 1;
	else if (key == 's')
		cam_pos_y -= 1;

	// look at
	else if (key == 'a')
		// rotera höger
		cam_look_angle += 3.14 * 0.1;
	else if (key == 'd')
			// rotera höger
		cam_look_angle -= 3.14 * 0.1;

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
	glutKeyboardFunc(KeyHandler);

	glutMainLoop();
	exit(0);
}
