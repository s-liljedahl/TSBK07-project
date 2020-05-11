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

#include "terrain.h"

float scaling_factor = 1.0f;

GLfloat *vertexArray;
GLuint program, program_fish, program_shark, program_ship, program_grass;
Model *tm, *fish_player, *shark, *ship, *grass;
GLuint tex1;
TextureData ttex; // terrain

void init_terrain(mat4 projectionMatrix)
{
	LoadTGATextureSimple("resources/sand.tga", &tex1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex1); // Bind Our Texture tex1
	printError("init sand");

	// Load and compile shader
	program = loadShaders("shaders/terrain.vert", "shaders/terrain.frag");
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	printError("init shader");

	// Load terrain data
	LoadTGATextureData("resources/fft-terrain.tga", &ttex);
	tm = GenerateTerrain(&ttex);
	printError("init terrain");

	// Load fish and compile shader
	fish_player = LoadModelPlus("resources/fish.obj");
	program_fish = loadShaders("shaders/fish.vert", "shaders/fish.frag");
	glUniformMatrix4fv(glGetUniformLocation(program_fish, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	printError("init fish");

	// Load shark and compile shader
	shark = LoadModelPlus("resources/shark.obj");
	program_shark = loadShaders("shaders/sphere.vert", "shaders/shark.frag");
	glUniformMatrix4fv(glGetUniformLocation(program_shark, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	printError("init shark");
}

void init_ship(mat4 projectionMatrix) {
	// Load ship and compile shader
	ship = LoadModelPlus("resources/submarine.obj");
	program_ship = loadShaders("shaders/ship.vert", "shaders/ship.frag");
	glUseProgram(program_ship);
	printError("init ship 2");
	glUniformMatrix4fv(glGetUniformLocation(program_ship, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	printError("init ship 3");
}

void init_grass(mat4 projectionMatrix) {
	grass = LoadModelPlus("resources/grass.obj");
	printError("init grass 1");
	program_grass = loadShaders("shaders/grass.vert", "shaders/grass.frag");
	glUseProgram(program_grass);
	printError("init grass 2");
	glUniformMatrix4fv(glGetUniformLocation(program_grass, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	printError("init grass 3");
}

void draw_terrain(vec3 cameraPos, mat4 total, GLfloat time)
{
	//terrain
	glUseProgram(program);
	glUniform1f(glGetUniformLocation(program, "t"), time);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	glUniform3fv(glGetUniformLocation(program, "cameraPos"), 1, &cameraPos); // Fog effect
	DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");
}

float getFogFactor(vec3 p1, vec3 p2)
{
		float dx = p1.x - p2.x; 
		float dy = p1.y - p2.y;
		float dz = p1.z - p2.z;
		float d = sqrt(dx*dx + dy*dy + dz*dz);

		// printf("d %f", d);
    const float FogMax = 100.0;
    const float FogMin = 1.0;
    if (d>=FogMax) return 1;
    if (d<=FogMin) return 0;
    return 1 - (FogMax - d) / (FogMax - FogMin);
}

void draw_ship(mat4 total, vec3 cameraPos, GLfloat time) 
{
	float scale_fact = 2.0f;
	vec3 pos = {70.0f, getHeight(70.0f, 120.0f) + 1.5f, 120.0f};
	mat4 transform = T(pos.x, pos.y, pos.z);
	mat4 scale = S(scale_fact, scale_fact,scale_fact);
	mat4 res = Mult(total, Mult(transform, scale));

	GLfloat visibility = getFogFactor(cameraPos, pos);

	glUseProgram(program_ship);
	glUniform1f(glGetUniformLocation(program_ship, "t"), time);
	glUniform1f(glGetUniformLocation(program_ship, "visibility"), visibility);
	glUniform3fv(glGetUniformLocation(program_ship, "cameraPos"), 1, &cameraPos); // Fog effect
	glUniformMatrix4fv(glGetUniformLocation(program_ship, "shipMatrix"), 1, GL_TRUE, res.m);
	DrawModel(ship, program_ship, "inPosition", "inNormal", "inTexCoord");
}


void draw_grass(mat4 total, vec3 cameraPos, GLfloat time) 
{
	float scale_fact = 0.2f;
	vec3 pos = {100.0f, getHeight(100.0f, 80.0f) + 1.0f, 80.0f};
	mat4 transform = T(pos.x, pos.y, pos.z);
	mat4 scale = S(scale_fact, scale_fact,scale_fact);
	mat4 res = Mult(total, Mult(transform, scale));

	GLfloat visibility = getFogFactor(cameraPos, pos);

	glUseProgram(program_grass);
	glUniform1f(glGetUniformLocation(program_grass, "t"), time);
	glUniform1f(glGetUniformLocation(program_grass, "visibility"), visibility);
	glUniformMatrix4fv(glGetUniformLocation(program_grass, "shipMatrix"), 1, GL_TRUE, res.m);
	DrawModel(grass, program_grass, "inPosition", "inNormal", "inTexCoord");
}

void draw_shark(GLfloat t, mat4 total, vec3 cameraPos)
{
	int radius = 4;
	mat4 shark_transform, shark_scale, shark_res, shark_direction, shark_pos;
	float scale_fact = 0.2f;
	GLfloat ticker = (t / 3500);

	for (int i = 0; i < 2; i++) 
	{
		//Initiera position
		GLfloat x = (sin(i) * radius) + 100.0f;
		GLfloat z = (cos(i) * radius) + 100.0f;
		GLfloat y = getHeight(x, z) + 2.0f;

		shark_pos = T(x, y, z);

		//Initiera tid
		GLfloat shark_posx = sin(ticker) * radius;
		GLfloat shark_posz = cos(ticker) * radius;
		GLfloat shark_posy = getHeight(shark_posx, shark_posz) + (sin(ticker + i*M_PI/2)*0.1) + 2.0f;

		shark_transform = T(shark_posx, shark_posy, shark_posz);

		//Skala hajen
		shark_scale = S(scale_fact, scale_fact, scale_fact);

		shark_res = Mult(shark_pos, shark_scale);
		shark_res = Mult(shark_transform, shark_res);

		shark_res.m[0] = scale_fact;
		shark_res.m[1] = 0;
		shark_res.m[2] = 0;

		shark_res.m[4] = 0;
		shark_res.m[5] = scale_fact;
		shark_res.m[6] = 0;

		shark_res.m[8] = 0;
		shark_res.m[9] = 0;
		shark_res.m[10] = scale_fact; 

		shark_direction = Ry(M_PI/2 + ticker);
		shark_res = Mult(shark_res, shark_direction);

		vec3 pos = {x, y, z};
		GLfloat visibility = getFogFactor(cameraPos, pos);

		glUseProgram(program_shark);
		glUniform1f(glGetUniformLocation(program_shark, "visibility"), visibility);
		glUniformMatrix4fv(glGetUniformLocation(program_shark, "view"), 1, GL_TRUE, total.m);
		glUniformMatrix4fv(glGetUniformLocation(program_shark, "sphereMatrix"), 1, GL_TRUE, shark_res.m);
		DrawModel(shark, program_shark, "inPosition", "inNormal", "inTexCoord");
	}
}

void draw_fish(mat4 camMatrix, vec3 cameraPos, vec3 cameraFront, vec3 cameraUp, GLfloat time) 
{
	mat4 fish_scale, fish_res, fish_transform, fish_direction;
	float scale_fact = 0.009f;

	GLfloat Rx_init = cameraPos.x + cameraFront.x + cameraUp.x;
	GLfloat Ry_init = cameraPos.y + cameraFront.y + (sin(time/1000)*0.01);
	GLfloat Rz_init = cameraPos.z + cameraFront.z + cameraUp.z;

	fish_transform = T(Rx_init, Ry_init, Rz_init);
	fish_scale = S(scale_fact, scale_fact, scale_fact);
	fish_res = Mult(camMatrix, Mult(fish_transform, fish_scale));

	fish_res.m[0] = scale_fact;
	fish_res.m[1] = 0;
	fish_res.m[2] = 0;

	fish_res.m[4] = 0;
	fish_res.m[5] = scale_fact;
	fish_res.m[6] = 0;

	fish_res.m[8] = 0;
	fish_res.m[9] = 0;
	fish_res.m[10] = scale_fact;

	fish_direction = Mult(Rz(-M_PI/2), Ry(-M_PI/2));
	fish_res = Mult(fish_res, fish_direction); 

	glUseProgram(program_fish);
	glUniformMatrix4fv(glGetUniformLocation(program_fish, "fishMatrix"), 1, GL_TRUE, fish_res.m);
	DrawModel(fish_player, program_fish, "inPosition", "inNormal", "inTexCoord");
}

float getHeight(float x, float z)
{
	int texWidth = ttex.width;
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
		h1 = getVertexHeight(tm, intX, intZ, texWidth);
		h2 = getVertexHeight(tm, intX + 1, intZ, texWidth);
		h3 = getVertexHeight(tm, intX, intZ + 1, texWidth);
		// printf("H1 %f %f %f\n", h1, h2, h3);

		vec3 a = {0, h1, 0};
		vec3 b = {1, h2, 0};
		vec3 c = {0, h3, 1};

		newHeight = barryCentric(a, b, c, deltaX, deltaZ);
	}
	else
	{
		// lower triangle
		h1 = getVertexHeight(tm, intX + 1, intZ, texWidth);
		h2 = getVertexHeight(tm, intX + 1, intZ + 1, texWidth);
		h3 = getVertexHeight(tm, intX, intZ + 1, texWidth);
		// printf("H2 %f %f %f\n", h1, h2, h3);

		vec3 a = {1, h1, 0};
		vec3 b = {1, h2, 1};
		vec3 c = {0, h3, 1};

		newHeight = barryCentric(a, b, c, deltaX, deltaZ);
	}
	return newHeight;
}

Model *GenerateTerrain(TextureData *tex)
{
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width - 1) * (tex->height - 1) * 2;
	int x, z;

	vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount * 3);

	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
			// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width) * 3 + 0] = scaling_factor * x / 1.0;
			vertexArray[(x + z * tex->width) * 3 + 1] = scaling_factor * tex->imageData[(x + z * tex->width) * (tex->bpp / 8)] / 60.0;
			vertexArray[(x + z * tex->width) * 3 + 2] = scaling_factor * z / 1.0;
			// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width) * 2 + 0] = x; // (float)x / tex->width;
			texCoordArray[(x + z * tex->width) * 2 + 1] = z; // (float)z / tex->height;
		}

	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
			float h, h_left, h_right, h_up, h_down;
			vec3 N;

			h = vertexArray[(x + z * tex->width) * 3 + 1];

			if (z == tex->height - 1)
			{
				h_down = h;
			}
			else
			{
				h_down = vertexArray[(x + (z + 1) * tex->width) * 3 + 1];
			}

			if (z == 0)
			{
				h_up = h;
			}
			else
			{
				h_up = vertexArray[(x + (z - 1) * tex->width) * 3 + 1];
			}

			if (x == tex->height - 1)
			{
				h_right = h;
			}
			else
			{
				h_right = vertexArray[((x + 1) + z * tex->width) * 3 + 1];
			}

			if (x == 0)
			{
				h_left = h;
			}
			else
			{
				h_left = vertexArray[((x - 1) + z * tex->width) * 3 + 1];
			}

			N.x = h_right - h_left;
			N.z = h_down - h_up;
			N.y = 2.0;

			vec3 N_norm = Normalize(N);

			normalArray[(x + z * tex->width) * 3 + 0] = N_norm.x;
			normalArray[(x + z * tex->width) * 3 + 1] = N_norm.y;
			normalArray[(x + z * tex->width) * 3 + 2] = N_norm.z;
		}

	for (x = 0; x < tex->width - 1; x++)
		for (z = 0; z < tex->height - 1; z++)
		{
			// Triangle 1
			indexArray[(x + z * (tex->width - 1)) * 6 + 0] = x + z * tex->width;
			indexArray[(x + z * (tex->width - 1)) * 6 + 1] = x + (z + 1) * tex->width;
			indexArray[(x + z * (tex->width - 1)) * 6 + 2] = x + 1 + z * tex->width;
			// Triangle 2
			indexArray[(x + z * (tex->width - 1)) * 6 + 3] = x + 1 + z * tex->width;
			indexArray[(x + z * (tex->width - 1)) * 6 + 4] = x + (z + 1) * tex->width;
			indexArray[(x + z * (tex->width - 1)) * 6 + 5] = x + 1 + (z + 1) * tex->width;
		}

	// End of terrain generation

	// Create Model and upload to GPU:
	Model *model = LoadDataToModel(
			vertexArray,
			normalArray,
			texCoordArray,
			NULL,
			indexArray,
			vertexCount,
			triangleCount * 3);

	return model;
}

float getVertexHeight(Model *model, int x, int z, int texWidth)
{
	return model->vertexArray[((x - 1) + z * texWidth) * 3 + 1];
}

float barryCentric(vec3 p1, vec3 p2, vec3 p3, float xpos, float zpos)
{
	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	float l1 = ((p2.z - p3.z) * (xpos - p3.x) + (p3.x - p2.x) * (zpos - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (xpos - p3.x) + (p1.x - p3.x) * (zpos - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;
	return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}