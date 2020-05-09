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

vec3 direction;
vec3 cameraFront;

void mouse(int xpos, int ypos);