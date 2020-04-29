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

#include "skybox-loader.h"

GLuint program_sky;
GLuint *texture;
Model *model;

void init_skybox()
{
	model = LoadModelPlus("resources/skybox/skybox.obj");
	program_sky = loadShaders("shaders/skybox.vert", "shaders/skybox.frag");
	glUseProgram(program_sky);
	glActiveTexture(GL_TEXTURE1);
	glUniform1i(glGetUniformLocation(program_sky, "skyTex"), 1);
	LoadTGATextureSimple("resources/skybox/nz.tga", &texture);
	glBindTexture(GL_TEXTURE_2D, texture);		// Bind Our Texture tex1	
}

void draw_skybox(mat4 projectionMatrix, mat4 cameraMatrix, mat4 transformMatrix)
{
	glUseProgram(program_sky);
	glUniformMatrix4fv(glGetUniformLocation(program_sky, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniformMatrix4fv(glGetUniformLocation(program_sky, "transform"), 1, GL_TRUE, transformMatrix.m);
	glBindTexture(GL_TEXTURE_2D, texture); //sky texture
	glUniformMatrix4fv(glGetUniformLocation(program_sky, "view"), 1, GL_TRUE, cameraMatrix.m);
	DrawModel(model, program_sky, "inPosition", "inNormal", "inTexCoord");
}