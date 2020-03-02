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

GLfloat *vertexArray;
float scaling_factor = 1.0f;

Model* GenerateTerrain(TextureData *tex)
{
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	int x, z;

	printf("count :%d\n", triangleCount);

	vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);

	printf("bpp %d\n", tex->bpp);
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width) * 3 + 0] = scaling_factor * x / 1.0;
			vertexArray[(x + z * tex->width) * 3 + 1] = scaling_factor * tex->imageData[(x + z * tex->width) * (tex->bpp / 8)] / 10.0;
			vertexArray[(x + z * tex->width) * 3 + 2] = scaling_factor * z / 1.0;
// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)*2 + 0] = x; // (float)x / tex->width;
			texCoordArray[(x + z * tex->width)*2 + 1] = z; // (float)z / tex->height;
		}

	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
			float h, h_left, h_right, h_up, h_down;
			vec3 N;

			h = vertexArray[(x + z * tex->width)*3 + 1];

			if (z == tex->height - 1) {
				h_down = h;
			} else {
				h_down = vertexArray[(x + (z + 1) * tex->width)*3 + 1];
			}

			if (z == 0) {
				h_up = h;
			} else {
				h_up = vertexArray[(x + (z - 1) * tex->width)*3 + 1];
			}

			if (x == tex->height - 1) {
				h_right = h;
			} else {
				h_right = vertexArray[((x + 1) + z * tex->width)*3 + 1];
			}

			if (x == 0) {
				h_left = h;
			} else {
				h_left = vertexArray[((x - 1) + z * tex->width)*3 + 1];
			}

			N.x = h_right - h_left;
			N.z = h_down - h_up;
			N.y = 2.0;

			vec3 N_norm = Normalize(N);

			//printf("heights %f %f %f %f\n", h_right, h_left, h_down, h_up);
			// printf("Normal %f %f %f\n", N_norm.x, N_norm.y, N_norm.z);

			// Normal vectors. You need to calculate these.
			normalArray[(x + z * tex->width)*3 + 0] = N_norm.x;
			normalArray[(x + z * tex->width)*3 + 1] = N_norm.y;
			normalArray[(x + z * tex->width)*3 + 2] = N_norm.z;
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

float getVertexHeight(Model* model, int x, int z, int texWidth)
{
		return model->vertexArray[((x - 1) + z * texWidth) * 3 + 1];
}

float barryCentric(vec3 p1, vec3 p2, vec3 p3, float xpos, float zpos) {

	// p1, p2, p3 = triangle corners
	// xpos, zpos = coordinate inside triangle
	// printf("p1 %f %f %f\n", p1.x, p1.y, p1.z);
	// printf("p2 %f %f %f\n", p2.x, p2.y, p2.z);
	// printf("p3 %f %f %f\n", p3.x, p3.y, p3.z);
	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	// printf("det %f\n", det);
	float l1 = ((p2.z - p3.z) * (xpos - p3.x) + (p3.x - p2.x) * (zpos - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (xpos - p3.x) + (p1.x - p3.x) * (zpos - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;
	// printf("l %f %f %f\n", l1, l2, l3);
	return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}

float getHeight(float x, float z, Model *model, int texWidth)
{

	// printf("input %f %f\n", x, z);

	// antal grid quares ?
	float quadSize = texWidth / ((float)texWidth - 1.0f);
	// int quadSize = 1;

	// vilken quad/gridsquare
	int quadX = (int)floor(x / quadSize);
	int quadZ = (int)floor(z / quadSize);
	// printf("Quad numbers %d %d\n", quadX, quadZ);

	int intX = (int)x;
	int intZ = (int)z;
	// printf("int %d %d\n", intX, intZ);

	// where on the square/quad
	float deltaX = (float)(x - intX);
	float deltaZ = (float)(z - intZ);
	// printf("Delta %f %f\n", deltaX, deltaZ);

	vec3 a, b, c;
	float h1, h2, h3;

	// if outside bounds
	if (quadX >= texWidth || quadZ >= texWidth || quadX < 0 || quadZ < 0)
	{
		return 0.0f;
	}

	float newHeight;

	// upper or lower triangle, [ x < 1-Z ]
	if (deltaX <= (1 - deltaZ))
	{
		// upper triangle
		h1 = getVertexHeight(model, intX, intZ, texWidth);
		h2 = getVertexHeight(model, intX + 1, intZ, texWidth);
		h3 = getVertexHeight(model, intX, intZ + 1, texWidth);
		// printf("H1 %f %f %f\n", h1, h2, h3);

		vec3 a = {0, h1, 0};
		vec3 b = {1, h2, 0};
		vec3 c = {0, h3, 1};	

		newHeight = barryCentric(a, b, c, deltaX, deltaZ);
	}
	else
	{
		// lower triangle
		h1 = getVertexHeight(model, intX + 1, intZ, texWidth);
		h2 = getVertexHeight(model, intX + 1, intZ + 1, texWidth);
		h3 = getVertexHeight(model, intX, intZ + 1, texWidth);
		// printf("H2 %f %f %f\n", h1, h2, h3);

		vec3 a = {1, h1, 0};
		vec3 b = {1, h2, 1};
		vec3 c = {0, h3, 1};	

		newHeight = barryCentric(a, b, c, deltaX, deltaZ);
	}

	return newHeight;
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

	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 500.0);

	// Load and compile shader
	program = loadShaders("lab4-3.vert", "lab4-3.frag");
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

	// float height = getHeight(2.5f, 5.1f, tm, ttex.width);
	float height = getHeight(cam_pos_x, cam_pos_z, tm, ttex.width);
	printf("Height: %f\n", height);

	vec3 cam = {cam_pos_x, cam_pos_y, cam_pos_z};
	vec3 lookAtPoint = {sin(cam_look_angle), 0, cos(cam_look_angle)};
	camMatrix = lookAt(cam.x, height + 3, cam.z,
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
