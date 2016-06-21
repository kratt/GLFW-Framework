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

private:
	png_structp m_pngPtr;
	png_infop m_infoPtr;
	png_infop m_endInfo;

	png_byte* m_imageData;
	png_bytep* m_rowPointers;

	int m_width;
	int m_height;

	bool m_hasAlpha;
};

#endif 