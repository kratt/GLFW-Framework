#include "text_utils.h"

#include <glm/vec2.hpp>
#include <glm/gtx/norm.hpp>

#include <algorithm>
#include <iostream>


namespace utils {

	void unpackMonoBitmap(FT_Bitmap* bitmap, std::vector<unsigned char> &outData)
	{
		int width  = bitmap->width;
		int height = bitmap->rows;

		outData.resize(width * height);

		GLubyte *data = new unsigned char[width * height];

		for (int y = 0; y < bitmap->rows; ++y)
		{
			for (int byteIndex = 0; byteIndex < bitmap->pitch; ++byteIndex)
			{
				auto byte_value = bitmap->buffer[y * bitmap->pitch + byteIndex];
				int num_bits_done = byteIndex * 8;
				int row_start = y * bitmap->width + byteIndex * 8;

				for (int bitIndex = 0; bitIndex < std::min(8, bitmap->width - num_bits_done); ++bitIndex)
				{
					auto bit = byte_value & (1 << (7 - bitIndex));

					if (bit)
						outData[row_start + bitIndex] = 255;
					else
						outData[row_start + bitIndex] = 0;
				}
			}
		}
	}

	void emptyTexture(FT_Bitmap* bitmap, unsigned char** outData)
	{
		int width = bitmap->width;
		int height = bitmap->rows;

		GLubyte *data = new unsigned char[width * height];

		for (int x = 0; x < width; ++x)
		{
			for (int y = 0; y < height; ++y)
			{
				data[y*width + x] = 255;
			}
		}

		*outData = data;
	}

	//http://www.codersnotes.com/notes/signed-distance-fields/
	void distanceField(FT_Bitmap* bitmap, int border, std::vector<float> &outData)
	{
		// first unpack monochromatic bitmap
		std::vector<unsigned char> bitMapData;
		unpackMonoBitmap(bitmap, bitMapData);

		int width  = bitmap->width + 2 * border;
		int height = bitmap->rows  + 2 * border;

		outData.resize(width * height);

		std::vector<unsigned char> resizedBitMapData = std::vector<unsigned char>(width * height);
		for (int x = 0; x < width; ++x)
		{
			for (int y = 0; y < height; ++y)
			{
				unsigned char val = 0; 

				if (x >= border && x < width - border &&
					y >= border && y < height - border)
				{
					int idx = (y-border)*(width-2*border) + x-border;
					val = bitMapData[idx];
				}

				resizedBitMapData[y*width + x] = val;
			}
		}

		float minDist = std::numeric_limits<float>::max();
		float maxDist = std::numeric_limits<float>::lowest();

		std::vector<float> sdf = std::vector<float>(width * height);
		std::vector<bool> processed = std::vector<bool>(width *height);

		float spread = 16.0f;

		//compute signed distance field
		for (int x = 0; x < width; ++x)
		{
			for (int y = 0; y < height; ++y)
			{
				glm::vec2 pos = glm::vec2(x, y);
				auto val = resizedBitMapData[y*width + x];

				// inside texels (white) have a negative sign
				float sign = val > 0 ? -1.0f : 1.0f;

				// found nearest opposite texel
				float dist = std::numeric_limits<float>::max();

				bool found = false;
				for (int s = 0; s < width; ++s)
				{
					for (int t = 0; t < height; ++t)
					{
						glm::vec2 curPos = glm::vec2(s, t);
						auto curVal = resizedBitMapData[t*width + s];

						if (curVal == val)
							continue;

						float curDist = glm::length(curPos - pos);

						if (curDist < spread)
						{

							dist = std::min(dist, curDist);
							found = true;
						}
					}
				}

				processed[y*width + x] = found;

				if (found)
				{
					float newVal = sign * dist;
					sdf[y*width + x] = newVal;

					minDist = std::min(minDist, newVal);
					maxDist = std::max(maxDist, newVal);
				}
				else
				{
					sdf[y*width + x] = sign;
				}
			}
		}

		std::cout << "min: " << minDist << " , max:" << maxDist << std::endl;

		// normalize data and write output
		for (int x = 0; x < width; ++x)
		{
			for (int y = 0; y < height; ++y)
			{
				int idx = y*width + x;
				auto val = sdf[idx];

				if (processed[idx]) {
					outData[idx] = (val - minDist) / (maxDist - minDist);// *255;
				}
				else if (val < 0) {
					outData[idx] = 0.0f;
				}
				else {
					outData[idx] = 1.0f;
				}
			
			}
		}
	}

}