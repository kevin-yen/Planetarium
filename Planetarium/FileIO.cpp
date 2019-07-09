/*
 *	Copyright (c) 2008 Kevin H. Yen.
 *	All rights reserved.
 *
 *	FileIO.cpp
 *
 * Contains the functions for importing and exporting files,
 * and contains data structures for usage and storage. 
 *
 */

/* Standard C/C++ Libraries */
#include <iostream>
#include <fstream>

#include <assert.h>

#include "FileIO.h"

// CLASS - ByteArray
ByteArray::ByteArray(int aSize)
{	
	released = false;
	bArray = new char[aSize];
	arraySize = aSize;
}
ByteArray::~ByteArray( )
{
	delete [] bArray;
	bArray = NULL;
}
char *ByteArray::release( )
{
	released = true;
	if(bArray != NULL)
		return bArray;
	else
		return NULL;
}		
char *ByteArray::get()
{
	return bArray;
}
void ByteArray::reset( )
{
	delete [] bArray;
	bArray = NULL;
	released = false;
} 
void ByteArray::put(char value, int index)
{
	bArray[index] = value;
}
char ByteArray::operator[](int index)
{
	if(bArray)
		return bArray[index];
	else
		return NULL;
}

// Image structure.
Image::Image(int btmapWidth, int btmapHeight, short btmapBits, char *btmapPixels) :
			 Width(btmapWidth), Height(btmapHeight), Bits(btmapBits)
{	
	Pixels = new char[btmapWidth * btmapHeight * 3];
	
	assert(btmapPixels != NULL);
	assert(Pixels != NULL);

	for(int i = 0; i < btmapWidth * btmapHeight * 3; i ++)
		Pixels[i] = btmapPixels[i];
}
Image::~Image()
{
	delete [] Pixels;
}



// Read(Int/Char/Short)
// Function for reading data.
int ReadInt(ifstream& DataStream)
{
	char buffer[4];

	DataStream.read(buffer, 4);

	return (int) (( (unsigned char) buffer[3] << 24 ) |
				  ( (unsigned char) buffer[2] << 16 ) |
				  ( (unsigned char) buffer[1] << 8 ) |
				  ( (unsigned char) buffer[0] ));
}

char ReadChar(ifstream& DataStream)
{
	char buffer;

	DataStream.read(&buffer, 1);

	return (char) buffer;
}

short ReadShort(ifstream& DataStream)
{
	char buffer[2];

	DataStream.read(buffer, 2);

	return (short) (( (unsigned char) buffer[1] << 16 ) |
					( (unsigned char) buffer[0] ));
}

float ReadFloat(ifstream& DataStream)
{
	char buffer[4];
	float value;

	DataStream.read(buffer, 4);

	memcpy(&value, &buffer, 4);

	return value;
}

void ReadFileName(ifstream& DataStream, char FilePath[])
{
	char buffer;

	for(int index = 0; index < FILE_LENGTH; index ++)
	{
		DataStream.read(&buffer, 1);
		FilePath[index] = buffer;
	}
}

// Function to load Bitmap file.
Image *LoadBmp(const char* FileName)
{
	// BMP File Header
	char	bmpID[2];
	int		bmpFileSize;
	int		bmpDataOffset;

	// BMP V3-Specific Info Header Values
	int		bmpHeaderSize;
	int		bmpWidth;
	int		bmpHeight;
	short	bmpColorPlane;
	short	bmpBitsPerPixel;
	int		bmpCompression;
	int		bmpImageSize;
//	int		bmpHorRes;
//	int		bmpVerRes;
//	int		bmpColorPalette;
//	int		bmpMajorColors;

	bool Opened;

	ifstream DataStream;

	DataStream.open(FileName, ios::binary);
	assert(DataStream.is_open( ));

	DataStream.read(bmpID, 2);
	if( (bmpID[0] != 'B') || (bmpID[1] != 'M') )
	{
		assert( (bmpID[0] == 'B') && (bmpID[1] == 'M') );
		return NULL;
	}

  	bmpFileSize = ReadInt(DataStream);
	DataStream.ignore(4);
	bmpDataOffset = ReadInt(DataStream);

	bmpHeaderSize = ReadInt(DataStream);
	
	switch(bmpHeaderSize)
	{
	case 40:	/* Windows V3 */
		bmpWidth = ReadInt(DataStream);
		bmpHeight = ReadInt(DataStream);
		bmpColorPlane = ReadShort(DataStream);
		bmpBitsPerPixel = ReadShort(DataStream);
		bmpCompression = ReadInt(DataStream);
		// Cannot have any Compression
		if(bmpCompression != 0)
			return NULL;	
		bmpImageSize = ReadInt(DataStream);
		break;

	case 12:	/* Window V1 */
		bmpWidth = (int) ReadShort(DataStream);
		bmpHeight = (int) ReadShort(DataStream);
		bmpColorPlane = ReadShort(DataStream);
		bmpBitsPerPixel = ReadShort(DataStream);
		break;

	default:	/* Other Bitmap Formats */
		return NULL;
		break;
	}

	if(!bmpImageSize)
	{
		int bytesPerRow = (((bmpBitsPerPixel * bmpWidth) + 31) / 32) * 4;
		bmpImageSize = bytesPerRow * bmpHeight;
	}

	ByteArray RawData(bmpImageSize);

	DataStream.seekg(bmpDataOffset, ios::beg);
	DataStream.read(RawData.get( ), bmpImageSize);
	
	ByteArray FormatData(bmpImageSize);
	
	for(int i = 0; i < bmpHeight; i ++)	{
		for(int j = 0; j < bmpWidth; j ++)	{
			for(int k = 0; k < 3; k ++)	{
				FormatData.put(RawData[(bmpImageSize / bmpHeight) * i + 3 * j + (2 - k)],
								3 * (bmpWidth * i + j) + k);
	}	}	   }

	DataStream.close();

	return new Image(bmpWidth, bmpHeight, bmpBitsPerPixel, FormatData.release( ));
}
//int LoadPng(char*, image*);
                                                           