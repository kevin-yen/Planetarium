// OpenGlMain.cpp

// Contains the main function for drawing 
// and rendering graphics using OpenGl.

#include <math.h>
#include <assert.h>
#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>

#include "OpenGlMain.h"
#include "AstrObj.h"
#include "FileIO.h"

extern HWND		hWnd;

static vector3 locCamera;
static vector2 angCamera;
static GLuint  texID;

static float lightPos_0[] = {0.0f, 0.0f, 0.0f, 1.0f};
static float lightAmb_0[] = {0.01f, 0.01f, 0.01f, 1.0f};
static float lightDif_0[] = {1.0f, 1.0f, 1.0f, 1.0f};

SolSys *starSystem;

// glfReisze :	Called when the window is resized
//				to re-adjest the viewport and point of view.
void giResize(int height, int width)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity( );

	glViewport(0, 0, width, height);
	gluPerspective(45.0f,(double) width / height, 0.01f, 1000.0f);
	gluLookAt(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity( );
}

// glfInit :	Initialize main utilities (Lighting for example) of OpenGL.
void giInit( ) 
{
	bool fFile = true;;

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	
	starSystem = AsObLoadFile( );
}

// glfKeyProc :	Function for handling all procedures 
//				when a key is pressed.
void giKeyProc(bool* keys)
{
	// Camera Movements :
	// Rotating
	if(keys[VK_RIGHT])
		angCamera[phi] = angCamera[phi] - 1.0f * rotate;
	if(keys[VK_LEFT])
		angCamera[phi] = angCamera[phi] + 1.0f * rotate;

	if(keys[VK_UP])
		angCamera[theta] = angCamera[theta] + 1.0f * rotate;
	if(keys[VK_DOWN])
		angCamera[theta] = angCamera[theta] - 1.0f * rotate;

	// Translating
	if(keys['S']) {
		if(keys[VK_SHIFT])
		{
			locCamera[x] = locCamera[x] + ( (speed * 100) * sin(angCamera[phi] * PI_180));
			locCamera[z] = locCamera[z] + ( (speed * 100) * cos(angCamera[phi] * PI_180));
		}
		else if(keys[VK_CONTROL])
		{
			locCamera[x] = locCamera[x] + ( (speed / 10) * sin(angCamera[phi] * PI_180));
			locCamera[z] = locCamera[z] + ( (speed / 10) * cos(angCamera[phi] * PI_180));
		}
		else
		{	locCamera[x] = locCamera[x] + (speed * sin(angCamera[phi] * PI_180));
			locCamera[z] = locCamera[z] + (speed * cos(angCamera[phi] * PI_180));
		}
	}
	if(keys['W']) {
		if(keys[VK_SHIFT])
		{
			locCamera[x] = locCamera[x] - ( (speed * 100) * sin(angCamera[phi] * PI_180));
			locCamera[z] = locCamera[z] - ( (speed * 100) * cos(angCamera[phi] * PI_180));
		}
		else if(keys[VK_CONTROL])
		{
			locCamera[x] = locCamera[x] - ( (speed / 10) * sin(angCamera[phi] * PI_180));
			locCamera[z] = locCamera[z] - ( (speed / 10) * cos(angCamera[phi] * PI_180));
		}
		else
		{	locCamera[x] = locCamera[x] - (speed * sin(angCamera[phi] * PI_180));
			locCamera[z] = locCamera[z] - (speed * cos(angCamera[phi] * PI_180));
		}
	}

	if(keys['A']) {
		locCamera[x] = locCamera[x] - (speed * cos( -(angCamera[phi] * PI_180) ));
		locCamera[z] = locCamera[z] - (speed * sin( -(angCamera[phi] * PI_180) ));
	}
	if(keys['D']) {
		locCamera[x] = locCamera[x] + (speed * cos( -(angCamera[phi] * PI_180) ));
		locCamera[z] = locCamera[z] + (speed * sin( -(angCamera[phi] * PI_180) ));
	}

	if(keys['R'])
		locCamera[y] = locCamera[y] + speed * 1.0f;
	if(keys['F'])
		locCamera[y] = locCamera[y] - speed * 1.0f;

	// Reset button.
	if(keys[VK_HOME]) {
		locCamera[x] = 0.0f;
		locCamera[y] = 0.0f;
		locCamera[z] = 5.0f;
		angCamera[phi] = 0.0f;
		angCamera[theta] = 0.0f;
	}
}

// glfMain :	Main routine for all the drawing and
//				graphics rendering of the program.
void GraphicsMain()
{
	glClear(GL_COLOR_BUFFER_BIT |
			GL_DEPTH_BUFFER_BIT);
	glLoadIdentity( );

	float colW[] = {1.0f, 1.0f, 1.0f};
	
	// Move Camera to Position
	giMoveCamera(locCamera, angCamera);

	for(int index = 0; index < starSystem->ReleaseCntPlanet( ); index ++)
	{
		starSystem->planets[index].UpdatePlt(0.01f);
		starSystem->planets[index].UpdateMoons(0.01f);
	}

	for(int index = 0; index < starSystem->ReleaseCntPlanet( ); index ++)
	{
		starSystem->planets[index].DrawPlt( );
		starSystem->planets[index].DrawMoons( );
	}

	// Create light
	giCreateLight(GL_LIGHT0, lightPos_0, lightAmb_0, lightDif_0);

	// LightCube
	glDisable(GL_LIGHTING);
	glPushMatrix( );
		glTranslatef(0.0f, 0.0f, 0.0f);
		giDrawCube(0.5f, colW);
	glPopMatrix( );
	glEnable(GL_LIGHTING);

//	glFlush( );
}

// glfCreateLight :	This create a light source and input it's
//					parameters (position, ambient, and diffuse).
void giCreateLight(GLenum light, float* position, float* ambient, float* diffuse)
{
	glEnable(light);

	glLightfv(light, GL_POSITION, (GLfloat *) position);
	glLightfv(light, GL_AMBIENT, (GLfloat *)  ambient);
	glLightfv(light, GL_DIFFUSE, (GLfloat *)  diffuse);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
}

// glfCreateTexture : This create a texture and returns it's ID.
uint giCreateTexture(Image *imageTexture)
{
	GLuint textureID;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 
				 0, GL_RGB,
				 imageTexture->Width,
				 imageTexture->Height,
				 0, GL_RGB,
				 GL_UNSIGNED_BYTE,
				 imageTexture->Pixels);

	return (uint) textureID;
}

// glfMoveCamera :	This move all the models in the scene
//					to match the movement of a camera.
void giMoveCamera(vector3 location, vector2 angles)
{
	glRotatef(-angles[phi], 0.0f, 1.0f, 0.0f);
	glRotatef(-angles[theta], cos((float) -(angles[phi] * PI_180) ) , 0.0f, sin((float) -(angles[phi] * PI_180) ));

	glTranslatef(-location[x], -location[y], -location[z]);
}

// glfDrawSquare :	This draws a square, it's arguement
//					are the width of the square (width)
//					and the color of the square (color).
void giDrawSquare(float width, float* color)
{
	glPushMatrix();

	glColor3fv( (GLfloat *) color);
	glBegin(GL_POLYGON);

		glNormal3f(0.0f, 0.0f, -1.0f);
		glVertex3f(width/2, width/2, 0.0f);

		glNormal3f(0.0f, 0.0f, -1.0f);
		glVertex3f(-width/2, width/2, 0.0f);
		
		glNormal3f(0.0f, 0.0f, -1.0f);
		glVertex3f(-width/2, -width/2, 0.0f);
		
		glNormal3f(0.0f, 0.0f, -1.0f);
		glVertex3f(width/2, -width/2, 0.0f);

	glEnd( );

	glPopMatrix();
}

// glfDrawCube :	Draws a cube.
void giDrawCube(float width, float* color)
{
	float half = width / 2.0f;

	glPushMatrix( );
		glTranslatef(0.0f, 0.0f, -half);
		giDrawSquare(width, color);
	glPopMatrix( );
	
	glPushMatrix( );
		glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.0f, 0.0f, -half);
		giDrawSquare(width, color);
	glPopMatrix( );
	
	glPushMatrix( );
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.0f, 0.0f, -half);
		giDrawSquare(width, color);
	glPopMatrix( );

	glPushMatrix( );
		glRotatef(-90.0f, 1.0f ,0.0f ,0.0f);
		glTranslatef(0.0f, 0.0f, -half);
		giDrawSquare(width, color);
	glPopMatrix( );

	glPushMatrix( );
		glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
		glTranslatef(0.0f, 0.0f, -half);
		giDrawSquare(width, color);
	glPopMatrix( );

	glPushMatrix( );
		glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
		glTranslatef(0.0f, 0.0f, -half);
		giDrawSquare(width, color);
	glPopMatrix( );

}
/*	The Following are Notes
	=======================

	Camera and Object Transformation Routine
	----------------------------------------

	1. Get camera rotation and translation*.
	   (Get camera traslation based on current
	   rotation)
	2. Get object rotation and translation.
	3. Execute camera rotation.
	4. Combine executions of the negative camera 
	   translation, object translation, and 
	   object location.
		
   *The camera we refer to is not actually the 
   viewing system, it's the transformation of 
   object to make an illusion that there's a camera.
			
	Sucessful Camera Transformation Check List
	------------------------------------------

	Fundamental :
	[X] Do I have a functioning camera rotational 
		system in the x-z plane.*
	[ ] Do I have a functioning camera rotational
		system in all planes.
	[X] Do I have a functioning camera translation
		system in the z axis.*
	[X] Do I have a functioning camera translation
		system in the x axis.*
	[ ] Do I have a functioning camera translation
		system in all axis and planes.

	Camera Style :
	[X] X-Z directional movements with linear Y
		movements.
	[ ] Circular movement around a point with Z
		movement going inward.

	Tweaking :
	[ ] Is my camera rotational system not too fast
		or slow.
	[ ] Is my camera traslation system not too fast
		or slow.

	Advanced :
	[ ] Do my camera rotational system mimic real 
		camera/viewing systems. Is the turing of the
		camear too abrubt (sudden acceleration).
	[ ] Do my camera traslation system mimic real
		translation systems (walking, driving).
*/