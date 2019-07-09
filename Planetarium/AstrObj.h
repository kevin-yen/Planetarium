/*
 *	Copyright (c) 2008 Kevin H. Yen.
 *	All rights reserved.
 *
 *	AstrObj.h
 *
 *	Contains the classes prototypes for drawing 
 *	and initializing sphere representing astronomical
 *	objects.
 *
 */

/* Windows 32 and Opengl Libraries */
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

/* Programmer generated Functions */
#include "FileIO.h"

/* Make sure there's no re-declaration */
#ifndef ASTROBJ_H
#define ASTROBJ_H	

/* Constant value defination */
#ifndef	FILE_LENGTH
#define FILE_LENGTH	50
#endif	// FILE_LENGTH

#ifndef PI_180
#define PI_180 0.017455329251994f
#endif  // PI_180

/* Type define declarations */
typedef	unsigned int Uint;

/* Constant value declarations */
const int   DETAIL = 50;
const float AU     = 100.0f;

const float KM_AU  = (float) 0.00000000668f;

const float RATIO = 0.1f;			 

/* Classes declarations */
class ObtObj
{
public:
	float objRadius;
	float orbitRadius;
	float orbitPeriod;
	float rotatePeriod;

	float axialTilt;

	char  fileLocTex[FILE_LENGTH];
	Uint textureId;

	GLUquadric *sphere;

	void SetTexFileLoc(char *);

public:
	float revolve;
	float rotate;

	ObtObj( );
	ObtObj(float, float, float, 
		   float, float, char[]);
	~ObtObj( );

	ObtObj & operator =(ObtObj &);

	void InitObtObj(float, float, float,
					float, float, char[]);

	void LoadTexture(char []);
};


class Moon: public ObtObj
{
public:
	Moon(float, float, float, 
		 float, float, char *);
	Moon( );
	~Moon( );

	void InitMoon(float, float, float,
				  float, float, char[]);

	Moon & operator =(Moon &);

	void Update(float);
	void Draw(float, float);
}; 

class Planet: public ObtObj
{
private:
	float eccentricity;
	
	int   isRing;
	float innerRingRadius;
	float outerRingRadius;

	float refreshRate;
	
	char  fileLocRing[FILE_LENGTH];
	Uint ringId;

	GLUquadric *sphere;

	bool isMoon;
	int  cntMoon;
	Moon *moons;

	void AllocMoon();
	void SetTexFileLoc(char []);
	void SetRngFileLoc(char []);

public:   
	Planet(float, float, float, 
		   float, float, char []);
	Planet( );
	~Planet( );

	void InitPlanet(float, float, float, 
					float, float, char []);

	Planet & operator=(Planet &);

	void CreateMoon(float, float, float, 
					 float, float, char []);

	void UpdatePlt(float);
	void DrawPlt( );
	
	void UpdateALL(float);
	void DrawALL( );

	void UpdateMoons(float);
	void DrawMoons( );

	bool GetIsMoon( );
	
	void GetPltCoord(float *x, float *y);
	void LoadRing(char[], float, float);
};

class SolSys
{
private:
	float starRadius;
	
	int cntPlanet;

public:
	Planet *planets;

	SolSys( );
	SolSys(float);
	SolSys(int, float, Planet *);
	~SolSys( );

	void SetStarRadius(float strRadius) {strRadius = strRadius;}
	void SetCntPlanet(int);

	Planet *ReleasePlanet( );
	int  ReleaseCntPlanet( );
	float ReleaseStrRadius( );


	void Draw(); 
};

/* Implememntation functions declarations */
void AsObDrawTexSphere(Uint, float, int, int);
void AsObDrawTexDisk(Uint, float, float, int);

/* Utility functions declarations */
void AsObFocusToPlanet(Planet *);
SolSys *AsObLoadFile( );

#endif	// ASTROBJ_H