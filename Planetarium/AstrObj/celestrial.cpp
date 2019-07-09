#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <conio.h>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <glut.h>
#endif

#include "FileIO.h"
#include "celestrial.h"


//	The function implamentation of class "planet"

	planet::planet(float y, float o, float r, float s)
	{	year = y; orbitRadius = o; rotationRate = s;
		planetRadius = r * ratio;  }
	
	void planet::planetUpdate(float period)
	{	//Update the status (where they are) of the planet
		revolution = (360.0f / (year * rotationRate)) * period;
		rotation = (360.0f / rotationRate) * period;

		while(revolution > 360)
			revolution = revolution - 360;
		while(rotation > 360)
			rotation = rotation - 360;
	}
	
	void planet::planetDraw()
	{	//Draw the planet
		glPushMatrix();
		glEnable(GL_TEXTURE_2D);

		//Make sure the base color is white
		glColor3f(1.0f, 1.0f, 1.0f);

		//Select the planet's texture
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//Put everything in position
		glScalef(-1.0f, -1.0f, 1.0f);
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		glRotatef(revolution, 0.0f, 0.0f, 1.0f);
		glTranslatef(AU * orbitRadius, 0.0f, 0.0f);
		glRotatef(rotation, 0.0f, 0.0f ,1.0f);

		//Make a sphere and draw it
		GLUquadric *sph = gluNewQuadric(); 
		gluQuadricTexture(sph,GL_TRUE); 
		gluSphere(sph,planetRadius,detail, detail); 
		gluDeleteQuadric(sph); 

		//Clean up
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
	//=================================================
	//The following function is modified from the 
	//original fuction that I've taken from a tutorial.
	//Thus, I don't claim credit for this fuction.
	//=================================================

	void planet::loadTex(Image* image)
	{
		glGenTextures(1, &texture); //Make room for our texture
		glBindTexture(GL_TEXTURE_2D, texture); //Tell OpenGL which texture to edit
		//Map the image to the texture
		glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
				 0,                            //0 for now
				 GL_RGB,                       //Format OpenGL uses for image
				 image->width, image->height,  //Width and height
				 0,                            //The border of the image
				 GL_RGB, //GL_RGB, because pixels are stored in RGB format
				 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
				                   //as unsigned numbers
				 image->pixels);               //The actual pixel data
	}

//	Function implimation of the class "satellite"

	satellite::satellite(int y, float o, float r, planet* hPlanet)
	{ year = y; orbitRadius = o * ratio; satelRadius = r; hostPlanet = hPlanet; }
	
	satellite::~satellite()
	{ delete hostPlanet; }
	
	void satellite::satelUpdate(int day)
	{	//Update the status (where they are) of the satellite
		revolution = (360.0 / year) * (float) day;
		
		while(revolution > 360)
			revolution = revolution - 360;
	}
	
	void satellite::satelDraw()
	{	//Draw the planet
		glPushMatrix();

		//Make sure the base color is white
		glColor3f(1.0f, 1.0f, 1.0f);

		// [SATELLITE TEXTURE NOT IMPLIMENTED]
		//Select the planet's texture
/*		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
*/
		//Put everything in position
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		glRotatef(hostPlanet->revolution, 0.0f, 0.0f, 1.0f);
		glTranslatef(AU * hostPlanet->orbitRadius, 0.0f, 0.0f);

		glRotatef(revolution, 0.0f, 0.0f, 1.0f);
		glTranslatef(AU * orbitRadius, 0.0f, 0.0f);

		//Make a sphere and draw it
		GLUquadric *sphere = gluNewQuadric(); 
//		gluQuadricTexture(sphere,GL_TRUE); 
		gluSphere(sphere,satelRadius,detail, detail); 
		gluDeleteQuadric(sphere);

		//Clean up
		glPopMatrix();
	}
/*
	void satellite::loadTex(Image* image) 
	{
		glGenTextures(1, &texture); //Make room for our texture
		glBindTexture(GL_TEXTURE_2D, texture); //Tell OpenGL which texture to edit
		//Map the image to the texture
		glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
				 0,                            //0 for now
				 GL_RGB,                       //Format OpenGL uses for image
				 image->width, image->height,  //Width and height
				 0,                            //The border of the image
				 GL_RGB, //GL_RGB, because pixels are stored in RGB format
				 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
				                   //as unsigned numbers
				 image->pixels);               //The actual pixel data
	} 
*/

	star::star(float r, float l, float h)
	{
		starRadius = r * ratio; starLumin = l;
		starHeat = h; 
		red = 0; blue = 0; green = 0;
	}
	void star::starDraw()
	{
		float starNoLumin[] = {0.0f, 0.0f, 0.0f, 1.0f};

		glPushMatrix();

		glColor3f(red, green, blue);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, &starLumin);

		GLUquadric *sphere = gluNewQuadric();
		gluSphere(sphere, starRadius, detail, detail);
		gluDeleteQuadric(sphere);

		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, starNoLumin);

		glPopMatrix();
	}

	void star::starUpdate()
	{
		red = 0;
		blue = 0;
		green = 0;
		findColor();
	}

	void star::findColor()
	{
		float wavelength;
		float increment;

		wavelength = (float) 2.89e6 / starHeat;

		if( (wavelength < 580) && (wavelength > 510) )
		{
				increment = wavelength - 510.0f;
				red = increment * ( 1.0f / (580.0f - 510.0f) );	
		}

		if(wavelength < 510)
		{
			red = 0.0f;

			//Slope of blue light starts at 490 um
			if(wavelength > 490)
			{
				increment = wavelength - 490.0f;
				blue = increment * ( 1.0f / (510.0f - 490.0f) );
			}
			else
				blue = 1.0f;
		}

		if(wavelength > 510)
		{
			blue = 0.0f;	

			if(wavelength > 580)
			{
				red = 1.0f;

				//Slope of green light starts at 580 um
				if(wavelength < 640)
				{
					increment = wavelength - 580.0f;
					green = increment * ( 1.0f / (640.0f - 580.0f) );
				}
				else
					green = 0.0f;
			}
		}

		if(wavelength < 490)
		{
			//Slope of the green light starts at 440 um
			if(wavelength > 440)
			{
				increment = wavelength - 440.0f;
				green = increment * ( 1.0f / (490.0f - 440.0f) );
			}
			if(wavelength < 440)
				green = 0.0f;
		}
	}

	
	void star::loadTex(Image* image) 
	{
		glGenTextures(1, &texture); //Make room for our texture
		glBindTexture(GL_TEXTURE_2D, texture); //Tell OpenGL which texture to edit
		//Map the image to the texture
		glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
				 0,                            //0 for now
				 GL_RGB,                       //Format OpenGL uses for image
				 image->width, image->height,  //Width and height
				 0,                            //The border of the image
				 GL_RGB, //GL_RGB, because pixels are stored in RGB format
				 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
				                   //as unsigned numbers
				 image->pixels);               //The actual pixel data
	} 
	