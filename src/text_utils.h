// 12.07.2016
// author: Julian Kratt

#ifndef TEXT_UTILS_H
#define TEXT_UTILS_H

#include "Global.h"
#include <ft2build.h>
#include FT_FREETYPE_H

#include <vector>

namespace utils {

	void distanceField(FT_Bitmap* bitmap, int border, std::vector<float> &outData);
	void unpackMonoBitmap(FT_Bitmap* bitmap, std::vector<unsigned char> &outData);
	void emptyTexture(FT_Bitmap* bitmap, unsigned char** outData);
}


#endif
