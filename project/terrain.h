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

void init_terrain(mat4 projectionMatrix);
void draw_terrain(vec3 cameraPos, mat4 total);
float getHeight(float x, float z);
Model *GenerateTerrain(TextureData *tex);
float getVertexHeight(Model *model, int x, int z, int texWidth);
float barryCentric(vec3 p1, vec3 p2, vec3 p3, float xpos, float zpos);