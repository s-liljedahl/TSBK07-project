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

GLuint program_sky;
GLuint sky_tex;
Model *skybox;

void init_skybox(Model *model, GLuint *program, GLuint *texture) {
  model = LoadModelPlus("skybox.obj");
  program = loadShaders("skybox.vert", "skybox.frag");
	LoadTGATextureSimple("SkyBox512.tga", &texture);
}


void draw_skybox(GLuint *program, Model *model, GLuint *texture, mat4 *transformMatrix, mat4 *cameraMatrix) {
	
	mat4 matrix1 = *cameraMatrix;
	mat4 matrix2 = *transformMatrix;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);

	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, matrix2.m);

	glBindTexture(GL_TEXTURE_2D, texture); //sky texture
	glDisable(GL_DEPTH_TEST);
	glUniformMatrix4fv(glGetUniformLocation(program, "transform"), 1, GL_TRUE, matrix1.m);
	DrawModel(model, program, "inPosition", "inNormal", "inTexCoord");
	glEnable(GL_DEPTH_TEST);
}