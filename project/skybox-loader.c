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
#include <stdio.h>
#include <string.h>

#include "skybox-loader.h"

GLuint program, texprogram;
Model *model[4]; // teapotModel, *sphereModel, *torusModel, *cubeModel;
Model *box[6];
int currentModelIndex = 0;

// Globals
// Data would normally be read from files
GLfloat vertices[6][6 * 3] =
	{
		{
			// +x
			0.5, -0.5, -0.5, // 1
			0.5, 0.5, -0.5,	 // 2
			0.5, 0.5, 0.5,	 // 6
			0.5, -0.5, 0.5,	 // 5
		},
		{
			// -x
			-0.5, -0.5, -0.5, // 0 -0
			-0.5, -0.5, 0.5,  // 4 -1
			-0.5, 0.5, 0.5,	  // 7 -2
			-0.5, 0.5, -0.5,  // 3 -3
		},
		{
			// +y
			0.5, 0.5, -0.5,	 // 2 -0
			-0.5, 0.5, -0.5, // 3 -1
			-0.5, 0.5, 0.5,	 // 7 -2
			0.5, 0.5, 0.5,	 // 6 -3
		},
		{
			// -y
			-0.5, -0.5, -0.5, // 0
			0.5, -0.5, -0.5,  // 1
			0.5, -0.5, 0.5,	  // 5
			-0.5, -0.5, 0.5	  // 4
		},
		{
			// +z
			-0.5, -0.5, 0.5, // 4
			0.5, -0.5, 0.5,	 // 5
			0.5, 0.5, 0.5,	 // 6
			-0.5, 0.5, 0.5,	 // 7
		},
		{
			// -z
			-0.5, -0.5, -0.5, // 0
			-0.5, 0.5, -0.5,  // 3
			0.5, 0.5, -0.5,	  // 2
			0.5, -0.5, -0.5,  // 1
		}};

GLfloat texcoord[6][6 * 2] =
	{
		{
			1.0,
			1.0,
			1.0,
			0.0, // left OK
			0.0,
			0.0,
			0.0,
			1.0,
		},
		{
			0.0,
			1.0, // right OK
			1.0,
			1.0,
			1.0,
			0.0,
			0.0,
			0.0,
		},
		{
			1.0,
			0.0, // top OK
			0.0,
			0.0,
			0.0,
			1.0,
			1.0,
			1.0,
		},
		{
			0.0,
			1.0,
			1.0,
			1.0,
			1.0,
			0.0, // bottom
			0.0,
			0.0,
		},
		{
			0.0,
			1.0,
			1.0,
			1.0,
			1.0,
			0.0, // back OK
			0.0,
			0.0,
		},
		{
			1.0,
			1.0,
			1.0,
			0.0, // front OK
			0.0,
			0.0,
			0.0,
			1.0,
		}};
GLuint indices[6][6] =
	{
		{0, 2, 1, 0, 3, 2},
		{0, 2, 1, 0, 3, 2},
		{0, 2, 1, 0, 3, 2},
		{0, 2, 1, 0, 3, 2},
		{0, 2, 1, 0, 3, 2},
		{0, 2, 1, 0, 3, 2}};

#define TEXTURE_OFFSET 0

char *textureFileName[6] =
	{
		"resources/skybox/textures/px.tga", // ok
		"resources/skybox/textures/nx.tga", // ok
		"resources/skybox/textures/py.tga", // ok
		"resources/skybox/textures/ny.tga", // ok
		"resources/skybox/textures/pz.tga", // ok
		"resources/skybox/textures/nz.tga", // ok
};

TextureData t[6];
void loadTextures()
{
	int i;
	glActiveTexture(GL_TEXTURE1); // Just make sure the texture unit match
	// Load texture data and create ordinary texture objects (for skybox)
	for (i = 0; i < 6; i++)
	{
		printf("Loading texture %s\n", textureFileName[i + TEXTURE_OFFSET]);
		LoadTGATexture(textureFileName[i + TEXTURE_OFFSET], &t[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
}

void skybox_init(mat4 projMatrix)
{
	int i;
	for (i = 0; i < 6; i++)
	{
		box[i] = LoadDataToModel(vertices[i], NULL, texcoord[i], NULL, indices[i], 4, 6);
	}
	texprogram = loadShaders("shaders/tex.vert", "shaders/tex.frag");
	glUniformMatrix4fv(glGetUniformLocation(texprogram, "projMatrix"), 1, GL_TRUE, projMatrix.m);
	glUniform1i(glGetUniformLocation(texprogram, "tex"), 1); // unit 1
	glEnable(GL_DEPTH_TEST);
	loadTextures();
}

void skybox_display(mat4 cameraMatrix)
{
	int i;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glUseProgram(texprogram);
	cameraMatrix.m[3] = 0;
	cameraMatrix.m[7] = 0;
	cameraMatrix.m[11] = 0;
	glUniformMatrix4fv(glGetUniformLocation(texprogram, "worldToViewMatrix"), 1, GL_TRUE, cameraMatrix.m);
	for (i = 0; i < 6; i++)
	{
		glBindTexture(GL_TEXTURE_2D, t[i].texID);
		DrawModel(box[i], texprogram, "inPosition", NULL, "inTexCoord");
	}
	glEnable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE1);
	glutSwapBuffers();
}