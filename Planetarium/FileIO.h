// FileIO.h

// Contains the functions for importing and exporting files,
// and contains data structures for usage and storage.

#include <iostream>
#include <fstream>

using namespace std;

#ifndef	FILEIO_H
#define FILEIO_H

/* Constant value defination */
#ifndef	FILE_LENGTH
#define FILE_LENGTH	50
#endif	//FILE_LENGTH

// Byte array class.
class ByteArray
{
private:
	char *bArray;
	bool released;

public:
	int  arraySize;

	ByteArray(int);
	~ByteArray( );

	char *release( );
	char *get( );
	void put(char, int);
	void reset( );

	char operator[](int);
};


// Image structure.
class Image
{
public:

	int Width;
	int Height;
	short Bits;
	char* Pixels;

	Image(int, int, short, char *);
	~Image();
};

// Function for reading data.
int ReadInt(ifstream&);
char ReadChar(ifstream&);
short ReadShort(ifstream&);
float ReadFloat(ifstream&);
void ReadFileName(ifstream&, char []);

// Function to load Bitmap file.
Image *LoadBmp(const char*);
//int LoadPng(char*, image*);



#endif