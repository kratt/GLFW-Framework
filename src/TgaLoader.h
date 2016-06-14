//Author: Thomas Lindemeier
//Date: 09.04.2014
//http://nehe.gamedev.net/tutorial/loading_compressed_and_uncompressed_tga's/22001/

#ifndef TGA_LOADER_H
#define TGA_LOADER_H

#include "Global.h"
#include <string>

class Texture;

class TgaLoader
{
	struct TGAHeader
	{
		GLubyte Header[12];									// TGA File Header
	};


	struct TGA
	{
		GLubyte		header[6];								// First 6 Useful Bytes From The Header
		GLuint		bytesPerPixel;							// Holds Number Of Bytes Per Pixel Used In The TGA File
		GLuint		imageSize;								// Used To Store The Image Size When Setting Aside Ram
		GLuint		temp;									// Temporary Variable
		GLuint		type;
		GLuint		Height;									//Height of Image
		GLuint		Width;									//Width ofImage
		GLuint		Bpp;									// Bits Per Pixel
	};


	TGAHeader tgaheader;									// TGA header
	TGA tga;												// TGA image data


	static GLubyte uTGAcompare[12];	// Uncompressed TGA Header
	static GLubyte cTGAcompare[12];	// Compressed TGA Header

	bool loadUncompressedTGA(Texture &, const std::string &, FILE *);	// Load an Uncompressed file
	bool loadCompressedTGA(Texture &, const std::string &, FILE *);		// Load a Compressed file

public:
	TgaLoader();
	~TgaLoader();

	bool loadTga(Texture & texture, const std::string & filename);
};

#endif // TGA_LOADER_H