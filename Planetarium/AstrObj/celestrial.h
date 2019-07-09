#ifndef CELESTRIAL_H
#define CELESTRIAL_H

#include "imageloader.h"
#include "GLmain.h"

//	---------------------------------------------
//	This class have all the necessary variables	 |
//	variable and functions for drawing, updating,|
//	and texturing a planet.						 |
//	---------------------------------------------

const float AU = 1;
const float KMinAU = 6.68e-9;
const float detail = 100;

const float ratio = 0.1;

class planet
{
private:
	float rotationRate;
	float planetRadius;
	GLuint texture;

public:
	int year;
	float orbitRadius;
	float revolution;
	float rotation;
	int id;
	
	planet(float y, float o, float r, float s);
	void planetUpdate(float period);
	void planetDraw();
	void loadTex(Image* image);
};

//	---------------------------------------------
//	This class have all the necessary variables	 |
//	variable and functions for drawing, updating,|
//	and [almost texturing] a satellite.			 |
//	---------------------------------------------


class satellite
{
private:
	int year;
	float satelRadius;

	GLuint texture;
	planet* hostPlanet;

public:
	float revolution;
	float orbitRadius;

	satellite(int y, float o, float r, planet* hPlanet);
	~satellite();
	void satelUpdate(int day);
	void satelDraw();
//	void loadTex(Image* image); 
}; 

//	---------------------------------------------
//	This class have all the necessary variables	 |
//	variable and functions for drawing, updating,|
//	and [almost texturing] a star.				 |
//	---------------------------------------------


class star
{
private:
	float   starRadius;
	GLfloat starLumin;
	float	starHeat;
	float	red;
	float	green;
	float	blue;
	GLuint texture;

	void findColor();

public:

	star(float r, float l, float h);
	void starUpdate();
	void starDraw(); 
	void loadTex(Image* image); 
};

#endif