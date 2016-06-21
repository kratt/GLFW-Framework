#ifndef PNG_LOADER_H
#define PNG_LOADER_H

#include "Global.h"
#include <string>

#include <png.h>
#include <string>


class PngLoader
{
public:
	PngLoader();
	PngLoader(const std::string fileName);
	~PngLoader();

	void load(const std::string fileName);

	unsigned char* imageData();

	int width() const;
	int height() const;
	bool hasAlpha() const;

	void clear();

	void read_png_file(char* file_name);
	void write_png_file(char* file_name);
	void process_file(void);
	void abort_(const char * s, ...);

	bool loadPngImage(const char *name, int &outWidth, int &outHeight, bool &outHasAlpha, GLubyte **outData);

private:
	png_structp m_pngPtr;
	png_infop m_infoPtr;
	png_infop m_endInfo;

	png_byte* m_imageData;
	png_bytep* m_rowPointers;

	int m_width;
	int m_height;

	bool m_hasAlpha;


	int x, y;

	int widthTest, heightTest;
	png_byte color_type;
	png_byte bit_depth;

	png_structp png_ptr;
	png_infop info_ptr;
	int number_of_passes;
	png_bytep * row_pointers;
};

#endif 