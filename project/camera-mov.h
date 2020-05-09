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

bool firstMouse;
float yaw;
float pitch;

float lastX;
float lastY;

vec3 cameraFront;
vec3 cameraUp;
vec3 cameraPos;
vec3 direction;

void mouse(int xpos, int ypos);
void SpecialKeyHandler(int key);
float radians(float degree);