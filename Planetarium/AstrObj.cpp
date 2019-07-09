/*
 *	Copyright (c) 2008 Kevin H. Yen.
 *	All rights reserved.
 *
 *	AstrObj.cpp
 *
 *	Contains the implementation functions for drawing 
 *	and initializing sphere representing astronomical
 *	objects.
 *
 */

/* Standard C/C++ Libraries */
#include <iostream>
#include <cmath>
#include <cassert>

/* Windows 32 and Opengl Libraries */
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

/* Programmer generated Functions */
#include "AstrObj.h"
#include "FileIO.h"

/* Class implementations */

// CLASS - ObtObj
// Set the File Location for the Texture Image
void ObtObj::SetTexFileLoc(char fileName[])
{
	for(int index = 0; index < FILE_LENGTH; index ++)
		fileLocTex[index] = fileName[index];
}

// Default Constructor
ObtObj::ObtObj( )
{
	sphere = gluNewQuadric( );
	gluQuadricTexture(sphere, GL_TRUE);

	objRadius		= 0.0f;
	orbitRadius		= 0.0f;
	orbitPeriod		= 0.0f;
	rotatePeriod	= 0.0f;
	axialTilt		= 0.0f;
	fileLocTex[0]	= '\0';

	rotate			= 0.0f;
	revolve			= 0.0f;
}

// Constructor
ObtObj::ObtObj(float ObjRad, float OrbRad, float OrbPer, 
			   float RotPer, float AxlTlt, char FilLoc[])
{	
	sphere = gluNewQuadric( );
	gluQuadricTexture(sphere, GL_TRUE);

	objRadius		= ObjRad;
	orbitRadius		= OrbRad;
	orbitPeriod		= OrbPer;
	rotatePeriod	= RotPer;
	axialTilt		= AxlTlt;
	for(int index = 0; index < FILE_LENGTH; index ++)
		fileLocTex[index] = FilLoc[index];

	rotate			= 0.0f;
	revolve			= 0.0f;
}

// Destructor
ObtObj::~ObtObj()
{
	for(int index = 0; index < FILE_LENGTH; index ++)
		fileLocTex[index] = '\0';

	if(sphere)
		gluDeleteQuadric(sphere);
}

// Assignment Operator Overload
ObtObj & ObtObj::operator =(ObtObj &Right)
{
	if(this == &Right)
		return *this;

	this->objRadius		= Right.objRadius;
	this->orbitPeriod	= Right.orbitPeriod;
	this->orbitRadius	= Right.orbitRadius;
	this->rotatePeriod	= Right.rotatePeriod;

	this->axialTilt		= Right.axialTilt;
	this->textureId		= Right.textureId;
	
	for(int index = 0; index < FILE_LENGTH; index ++)
		this->fileLocTex[index]	= Right.fileLocTex[index];

	return *this;
}

// Initialize the Value in the Class
void ObtObj::InitObtObj(float ObjRad, float OrbRad, float OrbPer, 
						float RotPer, float AxlTlt, char FilLoc[])
{
	objRadius		= ObjRad;
	orbitRadius		= OrbRad;
	orbitPeriod		= OrbPer;
	rotatePeriod	= RotPer;
	axialTilt		= AxlTlt;

	for(int index = 0; index < FILE_LENGTH; index ++)
	{	fileLocTex[index] = FilLoc[index];	}

	Image *imgTex	= LoadBmp( (const char *) &fileLocTex);

	assert(imgTex != NULL);

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 
					  GL_RGB,
				  	  imgTex->Width, imgTex->Height, 
				  	  GL_RGB, 
					  GL_UNSIGNED_BYTE, 
					  imgTex->Pixels);
}

// Load New Texture
void ObtObj::LoadTexture(char FilLoc[])
{
	for(int index = 0; index < FILE_LENGTH; index ++)
		fileLocTex[index] = '\0';

	for(int index = 0; index < FILE_LENGTH; index ++)
		fileLocTex[index] = FilLoc[index];

	Image *imgTex	= LoadBmp( (const char *) &fileLocTex);

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 
				 0, GL_RGB,
				 imgTex->Width, imgTex->Height, 
				 0, GL_RGB, 
				 GL_UNSIGNED_BYTE, 
				 imgTex->Pixels);
}

// CLASS - Moon
// Default Constructor
Moon::Moon( ) : ObtObj( ) {		}

// Constructor
Moon::Moon(float ObjRad, float OrbRad, float OrbPer, 
		   float RotPer, float AxlTlt, char FilLoc[]) :
		   ObtObj(ObjRad, OrbRad, OrbPer, RotPer, AxlTlt, FilLoc)
		   {	}

// Destructor
Moon::~Moon( ) {	}

// Initialize the Variable of the Class
void Moon::InitMoon(float ObjRad, float OrbRad, float OrbPer, 
					float RotPer, float AxlTlt, char FilLoc[])
{	InitObtObj(ObjRad, OrbRad, OrbPer, RotPer, AxlTlt, FilLoc);		}

// Assignment Operator Overload
Moon & Moon::operator =(Moon &Right)
{
	if(this == &Right)
		return *this;

	this->objRadius		= Right.objRadius;
	this->orbitPeriod	= Right.orbitPeriod;
	this->orbitRadius	= Right.orbitRadius;
	this->rotatePeriod	= Right.rotatePeriod;

	this->axialTilt		= Right.axialTilt;
	this->textureId		= Right.textureId;
	
	for(int index = 0; index < FILE_LENGTH; index ++)
		this->fileLocTex[index]	= Right.fileLocTex[index];

	return *this;
}

// Advance the State of the Moon
void Moon::Update(float incRate)
{
	rotate	= ( (360 / orbitPeriod) * incRate ) + rotate;
	revolve = ( (360 / (rotatePeriod * orbitPeriod) ) * incRate) + revolve;

	if(rotate > 360.0f)
		rotate	= rotate - 360.0f;
	if(revolve > 360.0f)
		revolve = revolve - 360.0f;
}

// Draw the Moon in it's Current State
void Moon::Draw(float xCoord, float zCoord)
{
	glColor3f(1.0f, 1.0f, 1.0f);

	glEnable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	glPushMatrix( );

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	glBindTexture(GL_TEXTURE_2D, textureId);

	glTranslatef(xCoord, 0.0f, zCoord);
	glTranslatef(cos(revolve * PI_180) * orbitRadius * AU,
				 0.0f,
				 sin(revolve * PI_180) * orbitRadius * AU);

	glRotatef(-(90.0f - axialTilt), 1.0f, 0.0f, 0.0f);
	glRotatef(rotate, 0.0f, 0.0f, 1.0f);

	gluSphere(this->ObtObj::sphere, objRadius, DETAIL, DETAIL);
	
	glPopMatrix( );

	glEnable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

// CLASS - Planet
// Set new texture file location to the current file location
void Planet::SetTexFileLoc(char FilLoc[])
{
	for(int index = 0; index < FILE_LENGTH; index ++)
		fileLocTex[index] = FilLoc[index];
}

// Set new ring file location to the current file lcoation
void Planet::SetRngFileLoc(char FilLoc[])
{
	for(int index = 0; index < FILE_LENGTH; index ++)
		fileLocRing[index] = FilLoc[index];
}

// Constructor
Planet::Planet(float ObjRad, float OrbRad, float OrbPer, 
			   float RotPer, float AxlTlt, char FilLoc[]) :
			   ObtObj(ObjRad, OrbRad, OrbPer, RotPer, AxlTlt, FilLoc)
			   {	}

// Default Constructor
Planet::Planet( )	{cntMoon = 0;}

// Destructor
Planet::~Planet( )
{// Delete the only dynamically alocated memory, moons

	switch(cntMoon)
	{
	case 0:	//No moons, nothing alocated
		/* Do Nothing */
		break;

	case 1:	//One moon, single moon alocated
		delete moons;
		break;

	default: //Everything else (assume positive), array alocated
		delete [] moons;
		break;
	}
}

// Initialize or overwrite a Planet
void Planet::InitPlanet(float ObjRad, float OrbRad, float OrbPer, 
						float RotPer, float AxlTlt, char FilLoc[])
{	
	InitObtObj(ObjRad, OrbRad, OrbPer, 
			   RotPer, AxlTlt, FilLoc);
}

// Assignment operator overload
Planet & Planet::operator =(Planet &plt)
{
	this->InitObtObj(plt.objRadius, plt.orbitRadius, plt.orbitPeriod,
					 plt.rotatePeriod, plt.axialTilt, plt.fileLocTex);

	switch(plt.cntMoon)
	{
	case 0:
		/* Do Nothing */
		break;

	case 1:
		this->CreateMoon(plt.moons->objRadius, plt.moons->orbitRadius, 
						 plt.moons->rotatePeriod, plt.moons->orbitPeriod, 
						 plt.moons->axialTilt, plt.moons->fileLocTex);
		break;

	default:
		for(int index = 0; index < cntMoon; index ++)
		{
			this->CreateMoon(plt.moons->objRadius, plt.moons->orbitRadius, 
							 plt.moons->rotatePeriod, plt.moons->orbitPeriod, 
							 plt.moons->axialTilt, plt.moons->fileLocTex);
		}
		break;
	}

	this->isRing		= plt.isRing;
	if(plt.isRing == 1)
	{
		this->innerRingRadius	= plt.innerRingRadius;
		this->outerRingRadius	= plt.outerRingRadius;
		this->SetRngFileLoc(plt.fileLocRing);
	}

	return *this;
}

// Allocates memory for and create a moon
void Planet::CreateMoon(float ObjRad, float OrbRad, float OrbPer, 
						float RotPer, float AxlTlt, char FilLoc[])
{
	Moon *tempMoon;

	switch(cntMoon)
	{
	case 0:
		moons = new Moon(ObjRad, OrbRad, OrbPer, RotPer, AxlTlt, FilLoc);
		cntMoon ++;
		break;

	case 1:
		tempMoon = new Moon;
		*tempMoon = *moons;
		delete moons;

		cntMoon ++;
		moons = new Moon[cntMoon];
		moons[0].InitMoon(ObjRad, OrbRad, OrbPer, RotPer, AxlTlt, FilLoc);

		moons[1] = *tempMoon;
		delete tempMoon;
		break;

	default:
		tempMoon = new Moon[cntMoon];
		for(int index = 0; index < cntMoon; index ++)
			tempMoon[index] = moons[index];
		delete [] moons;

		cntMoon ++;
		moons = new Moon[cntMoon];
		moons[0].InitMoon(ObjRad, OrbRad, OrbPer, RotPer, AxlTlt, FilLoc);

		for(int index = 1; index < cntMoon; index ++)
			moons[index] = tempMoon[index - 1];
		delete [] tempMoon;
		break;
	}

	moons[0].InitMoon(ObjRad, OrbRad, OrbPer, 
					  RotPer, AxlTlt, FilLoc);
}
	
// Update the state of the planet
void Planet::UpdatePlt(float incRate)
{
	rotate	= ( (360 / rotatePeriod) * incRate ) + rotate;
	revolve = ( (360 / (rotatePeriod * orbitPeriod)) * incRate) + revolve;

	if(rotate > 360.0f)
		rotate	= rotate - 360.0f;
	if(rotate < 0.0f)
		rotate  = 0.0f;
	if(revolve > 360.0f)
		revolve = revolve - 360.0f;
	if(revolve < 0.0f)
		revolve = 0.0f;
}
	
// Draw the planet
void Planet::DrawPlt( )
{
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glDisable(GL_BLEND);

	glPushMatrix( );

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, textureId);

	glTranslatef(cos(revolve * PI_180) * orbitRadius * AU,
				 0.0f,
				 sin(revolve * PI_180) * orbitRadius * AU);
	glRotatef(-(90.0f - axialTilt), 1.0f, 0.0f, 0.0f);
	glRotatef(rotate, 0.0f, 0.0f, 1.0f);

	gluSphere(this->ObtObj::sphere, objRadius, DETAIL, DETAIL);

	if(isRing == 1)
	{
		glEnable(GL_BLEND);
		AsObDrawTexDisk(ringId, innerRingRadius, outerRingRadius, DETAIL);
		glDisable(GL_BLEND);
	}

	glPopMatrix( );

	glEnable(GL_BLEND);
}
	
//	void UpdateALL(float);
//	void DrawALL( );

// Update the moons of the planet
void Planet::UpdateMoons(float incRate)
{
	if(cntMoon != 0)
	{
		for(int index = 0; index < cntMoon; index ++)
			moons[index].Update(incRate);
	}
}

// Draw the moons of the planet
void Planet::DrawMoons( )
{
	float xCoord, zCoord;

	GetPltCoord(&xCoord, &zCoord);

	for(int index = 0; index < cntMoon; index ++)
		moons[index].Draw(xCoord, zCoord);
}

// Return if there's a moon
bool Planet::GetIsMoon( )
{
	return isMoon;
}
	
// Return the x and y cooridinate
void Planet::GetPltCoord(float *x, float *y)
{
	*x = cos(revolve * PI_180) * orbitRadius * AU;
	*y = sin(revolve * PI_180) * orbitRadius * AU;
}

// Load the ring of the planet
void Planet::LoadRing(char fileLocRing[], float innerRngRad, float outerRngRad)
{
	isRing = 1;

	SetRngFileLoc(fileLocRing);

	innerRingRadius = innerRngRad;
	outerRingRadius = outerRngRad;

	Image *RingTex	= LoadBmp(fileLocRing);

	glGenTextures(1, &ringId);
	glBindTexture(GL_TEXTURE_2D, ringId);
	glTexImage2D(GL_TEXTURE_2D, 0,
					  GL_RGB, 
					  RingTex->Width, RingTex->Height, 0, 
					  GL_RGB, 
					  GL_UNSIGNED_BYTE, 
					  RingTex->Pixels);
}

// CLASS - SolSys

// Default Constructor
SolSys::SolSys( )	{	}

// Constructor
SolSys::SolSys(float stRadius)
{
	starRadius = stRadius;
}

// Constructor
SolSys::SolSys(int cntPlanets, float strRadius, Planet *dataPlanets)
{
	starRadius = strRadius;
	cntPlanet  = cntPlanets;


	planets = new Planet[cntPlanets];

	for(int index = 0; index < cntPlanets; index ++)
		planets[index] = dataPlanets[index];
}

// Destructor
SolSys::~SolSys( )
{
	delete [] planets;
}

// Release the pointer to memory containing 
// Planet datas
Planet *SolSys::ReleasePlanet( )
{
	return planets;
}

// Release the count of how many planets
int SolSys::ReleaseCntPlanet( )
{
	return cntPlanet;
}

// Release the radius of the star
float SolSys::ReleaseStrRadius( )
{
	return starRadius;
}

// Set the count of planet
void SolSys::SetCntPlanet(int CntPlanet)
{
	cntPlanet = CntPlanet;
}

// Implememntation functions declarations
void AsObDrawTexDisk(Uint texId, float innRngRad, float outRngRad, int slices)
{
	float ringWidth, ringAngle, triAngle;
	bool  invert = false;

	glColor4f(1.0f, 1.0f, 1.0f, 0.75f);

	glEnable(GL_TEXTURE_2D);

	glPushMatrix( );
	
	glBindTexture(GL_TEXTURE_2D, texId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	ringWidth = outRngRad - innRngRad;
	triAngle  = 360.0f / slices;	

	glBegin(GL_QUAD_STRIP);

	for(int index = 0; index < slices; index ++)
	{
		ringAngle = (float) index * (slices / 360.f);

		if(!invert)
			glTexCoord2d(0.0f, 0.0f);

		if(invert)
			glTexCoord2d(1.0f, 0.0f);

		glVertex3f(cos(ringAngle * PI_180 * AU) * outRngRad,
				   sin(ringAngle * PI_180 * AU) * outRngRad, 0.0f);

		if(!invert)
			glTexCoord2d(0.0f, 1.0f);
//			glTexCoord2d(sin(triAngle * PI_180), 1.0f);

		if(invert)
			glTexCoord2d(1.0f, 1.0f);
//			glTexCoord2d(1.0f - sin(triAngle * PI_180), 1.0f);

		glVertex3f(cos(ringAngle * PI_180 * AU) * innRngRad,
				   sin(ringAngle * PI_180 * AU) * innRngRad, 0.0f);

		invert = ~invert;
	} 

	glEnd( );

	glPopMatrix( );


	glDisable(GL_TEXTURE_2D);
}

void AsObFocusToPlanet(Planet *focusedPlanet)
{
	float xCoord, zCoord;

	focusedPlanet->GetPltCoord(&xCoord, &zCoord);

	glTranslatef(-xCoord, 0.0f, -zCoord);
//	glRotatef(-focusedPlanet->rotate, 0.0f, 1.0f, 0.0f);
}

SolSys *AsObLoadFile( )
{
	float FloatArrBuf[5];
	char  FileNameBuffer[FILE_LENGTH];
	int	  IntBuffer;

	int   cntPlt;
	int	  cntMon;
	
	SolSys systemData;
	
	ifstream dataStream;

	dataStream.open("Data\\SolarSysNotTested.data", ios::in | ios::binary);
	
	assert(dataStream.is_open( ) == true);

	systemData.SetStarRadius(ReadFloat(dataStream));

	IntBuffer = ReadInt(dataStream);

	systemData.SetCntPlanet(IntBuffer);

	systemData.planets = new Planet[IntBuffer];

	cntPlt = IntBuffer;

	for(int index = 0; index < cntPlt; index ++)
	{
		for(int secIndex = 0; secIndex < 5; secIndex ++)
			FloatArrBuf[secIndex] = ReadFloat(dataStream);

		ReadFileName(dataStream, FileNameBuffer);

		systemData.planets[index].InitPlanet(FloatArrBuf[0] * RATIO, FloatArrBuf[1], FloatArrBuf[3],
	 											 FloatArrBuf[2], FloatArrBuf[4], FileNameBuffer);

		for(int secIndex = 0; secIndex < FILE_LENGTH; secIndex ++)
			FileNameBuffer[secIndex] = '\0';

		IntBuffer = ReadInt(dataStream);

		if(IntBuffer == 1)
		{
			for(int secIndex = 0; secIndex < 2; secIndex ++)
				FloatArrBuf[secIndex] = ReadFloat(dataStream);
			
			ReadFileName(dataStream, FileNameBuffer);

			systemData.planets[index].LoadRing(FileNameBuffer, FloatArrBuf[0] * RATIO, FloatArrBuf[1] * RATIO);
		}

		IntBuffer = ReadInt(dataStream);

		cntMon = IntBuffer;

		for(int secIndex = 0; secIndex < cntMon; secIndex ++)
		{
			for(int thiIndex = 0; thiIndex < 5; thiIndex ++)
				FloatArrBuf[thiIndex] = ReadFloat(dataStream);

			ReadFileName(dataStream, FileNameBuffer);

			systemData.planets[index].CreateMoon(FloatArrBuf[0] * RATIO, FloatArrBuf[1], FloatArrBuf[3],
												 FloatArrBuf[2], FloatArrBuf[4], FileNameBuffer);
		}
	}

	dataStream.close( );
	
	return new SolSys(systemData.ReleaseCntPlanet( ), systemData.ReleaseStrRadius( ), systemData.ReleasePlanet( ));
}