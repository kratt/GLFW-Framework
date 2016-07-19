// 12.07.2016
// author: Julian Kratt

#ifndef TEXT_UTILS_H
#define TEXT_UTILS_H

#include "Global.h"
#include <ft2build.h>
#include FT_FREETYPE_H

#include <vector>

namespace utils {

	void distance_field(FT_Bitmap* bitmap, int border, std::vector<float> &outData);
	void distance_field_linear(FT_Bitmap* bitmap, int border, std::vector<float> &outData);
	void unpack_mono_bitmap(FT_Bitmap* bitmap, std::vector<unsigned char> &outData);
	void empty_texture(FT_Bitmap* bitmap, unsigned char** outData);

	std::vector<float> get_bitmap_data(FT_Bitmap* bitmap);

	/*void downSample(std::vector<float> inData, int width, int height, std::vector<float> outData, int targetWidth, int targetHeight);*/
}


#endif
