// OpenGlMain.h

// Contains the main function for drawing 

#include <windows.h>
#include <gl\gl.h>

#include "FileIO.h"

#ifndef OPENGLMAIN_H
#define OPENGLMAIN_H

// Constant value defination
#ifndef PI_180
#define PI_180 0.017455329251994f
#endif  // PI_180

// Creat four letter designation for common types.
typedef unsigned int uint;
typedef unsigned char byte;

// Create type [vector] to indicate float arrarys.
typedef float vector2[2];
typedef float vector3[3];
typedef float vector4[4];

// Create constants for mathmatical computation.
const float rotate	= 0.75f;
const float speed	= 0.125f;
const float update_rate = 0.01f;

// Create constants to better index and search with vector arrays.
const int x = 0;
const int y = 1;
const int z = 2;

const int theta = 0;
const int phi	= 1;

// Drawing functions.
void giDrawSquare(float, float*);
void giDrawCube(float, float*);

// Utility and setup functions.
void giCreateLight(GLenum light, float *, float *, float *);
uint giCreateTexture(Image *);
void giMoveCamera(vector3, vector2);

// Main and implementation functions for graphics.
void giResize(int, int);
void giInit( );
void giKeyProc(bool* keys);
void GraphicsMain();

#endif //_OPENGLMAIN_H