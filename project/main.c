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

#define pi 3.14

mat4 projectionMatrix;
float cam_pos_x = 2.0f;
float cam_pos_y = 5.0f;
float cam_pos_z = 8.0f;

float cam_look_x = 2.0f;
float cam_look_z = 2.0f;
float cam_look_angle = 0.0f;

GLfloat *vertexArray;
float scaling_factor = 1.0f;

// controls
bool firstMouse = true;
float yaw = -90.0f;
float pitch = 90.0f;

vec3 direction;
vec3 cameraPos = {100.0f, 5.0f,  100.0f};
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

Model* GenerateTerrain(TextureData *tex)
{
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	int x, z;

	vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);

	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width) * 3 + 0] = scaling_factor * x / 1.0;
			vertexArray[(x + z * tex->width) * 3 + 1] = scaling_factor * tex->imageData[(x + z * tex->width) * (tex->bpp / 8)] / 60.0;
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

	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	float l1 = ((p2.z - p3.z) * (xpos - p3.x) + (p3.x - p2.x) * (zpos - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (xpos - p3.x) + (p1.x - p3.x) * (zpos - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;

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
Model *tm, *fish_player;
// Reference to shader program
GLuint program, program_fish;
GLuint tex1;
TextureData ttex; // terrain

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

	// Load fish and compile shader
	fish_player = LoadModelPlus("fish.obj");

	program_fish = loadShaders("fish.vert", "fish.frag");
	glUniformMatrix4fv(glGetUniformLocation(program_fish, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

	// Load and compile terrain shader
	program = loadShaders("terrain.vert", "terrain.frag");
	LoadTGATextureSimple("sand.tga", &tex1);

	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniform1i(glGetUniformLocation(program, "tex1"), 0); // Texture unit 0

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1
	// Load terrain data
	LoadTGATextureData("fft-terrain.tga", &ttex);
	tm = GenerateTerrain(&ttex);
	
	printError("init terrain");
}

void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 total, modelView, camMatrix, camPlayer;

	printError("pre display");

	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	int radius = 30;
	GLfloat tickerSin = sin(t / 3000) * radius;
	GLfloat tickerCos = cos(t / 3000) * radius;

	camMatrix = lookAtv(cameraPos, VectorAdd(cameraPos, cameraFront), cameraUp);

	modelView = IdentityMatrix();
	total = Mult(camMatrix, modelView);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	
	// fish tranformations
	mat4 fish_scale, fish_res, fish_transform, mr4, fish_direction;

	GLfloat Rx_init = cameraPos.x + cameraFront.x + cameraUp.x;
	GLfloat Ry_init = cameraPos.y + cameraFront.y;
	GLfloat Rz_init = cameraPos.z + cameraFront.z + cameraUp.z;

	fish_transform = T(Rx_init, Ry_init, Rz_init);
	fish_scale = S(0.009f, 0.009f,0.009f);
	fish_res = Mult(camMatrix, Mult(fish_transform, fish_scale));

	fish_res.m[0] = 0.009;
	fish_res.m[1] = 0;
	fish_res.m[2] = 0;

	fish_res.m[4] = 0;
	fish_res.m[5] = 0.009;
	fish_res.m[6] = 0;

	fish_res.m[8] = 0;
	fish_res.m[9] = 0;
	fish_res.m[10] = 0.009;

	// mat3 mr3 = mat4tomat3(camMatrix); //för att endast ha med rotationerna
	// mr3 = TransposeMat3(mr3); //rotera motsatt mot kameran
	// mr4 = mat3tomat4(mr3); //tillbaks till mat4
	fish_direction = Mult(Rz(-M_PI/2), Ry(-M_PI/2));
	fish_res = Mult(fish_res, fish_direction); 

	//terrain
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");

	//fish
	glUseProgram(program_fish);
	glUniformMatrix4fv(glGetUniformLocation(program_fish, "fishMatrix"), 1, GL_TRUE, fish_res.m);
	DrawModel(fish_player, program_fish, "inPosition", "inNormal", "inTexCoord");

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
	GLfloat cam_y = getHeight(cameraPos.x, cameraPos.z, tm, ttex.width);
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
	glutCreateWindow ("TSBK07 Project");
	glutDisplayFunc(display);
	init ();
	glutTimerFunc(20, &timer, 0);
	glutSpecialFunc(SpecialKeyHandler);
	glutPassiveMotionFunc(mouse);
	glutMainLoop();
	exit(0);
}
