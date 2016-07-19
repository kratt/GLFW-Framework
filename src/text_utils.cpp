#include "text_utils.h"

#include <glm/vec2.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <algorithm>
#include <iostream>


namespace utils {

	void unpack_mono_bitmap(FT_Bitmap* bitmap, std::vector<unsigned char> &outData)
	{
		int width  = bitmap->width;
		int height = bitmap->rows;

		outData.resize(width * height);

		for (int y = 0; y < bitmap->rows; ++y)
		{
			for (int byteIndex = 0; byteIndex < bitmap->pitch; ++byteIndex)
			{
				auto byte_value = bitmap->buffer[y * bitmap->pitch + byteIndex];
				int num_bits_done = byteIndex * 8;
				int row_start = y * bitmap->width + byteIndex * 8;

				for (int bitIndex = 0; bitIndex < std::min(8, int(bitmap->width - num_bits_done)); ++bitIndex)
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

	void empty_texture(FT_Bitmap* bitmap, unsigned char** outData)
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

	std::vector<float> get_bitmap_data(FT_Bitmap * bitmap)
	{
		int width = bitmap->width;
		int height = bitmap->rows;

		std::vector<float> buffer(width * height, 0);

		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				
				int idx = y * width + x;
				buffer[idx] = float(bitmap->buffer[idx]) / 255;
				//std::cout << float(bitmap->buffer[idx]) << std::endl;
			}
		}
		return buffer;
	}

	//http://www.codersnotes.com/notes/signed-distance-fields/
	void distance_field(FT_Bitmap* bitmap, int border, std::vector<float> &outData)
	{
		// first unpack monochromatic bitmap
		std::vector<unsigned char> bitMapData;
		unpack_mono_bitmap(bitmap, bitMapData);

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

	glm::vec2 get_grid_val(std::vector<glm::vec2> &grid, int width, int height, int x, int y)
	{
		if (x >= 0 && y >= 0 && x < width && y < height)
			return grid[y*width + x];
		else
			return glm::vec2(9999, 9999);
	}

	void compare(std::vector<glm::vec2> &grid, int width, int height, int x, int y, int offsetx, int offsety)
	{
		glm::vec2 p = grid[y*width + x];
		glm::vec2 other = get_grid_val(grid, width, height, x + offsetx, y + offsety);

		other.x += offsetx;
		other.y += offsety;

		// square dist
		if (glm::dot(other, other) < glm::dot(p, p))
			grid[y*width + x] = other;
	}

	void generate_sdf(std::vector<glm::vec2> &grid, int width, int height)
	{
		// Pass 0
		for (int y = 0; y<height; y++)
		{
			for (int x = 0; x<width; x++)
			{
				compare(grid, width, height, x, y, -1,  0);
				compare(grid, width, height, x, y,  0, -1);
				compare(grid, width, height, x, y, -1, -1);
				compare(grid, width, height, x, y,  1, -1);
			}

			for (int x = width - 1; x >= 0; x--)
			{
				compare(grid, width, height, x, y, 1, 0);
			}
		}

		// Pass 1
		for (int y = height - 1; y >= 0; y--)
		{
			for (int x = width - 1; x >= 0; x--)
			{
				compare(grid, width, height, x, y, 1, 0);
				compare(grid, width, height, x, y, 0, 1);
				compare(grid, width, height, x, y, -1, 1);
				compare(grid, width, height, x, y, 1, 1);
			}

			for (int x = 0; x<width; x++)
			{
				compare(grid, width, height, x, y, -1, 0);
			}
		}
	}


	void distance_field_linear(FT_Bitmap* bitmap, int border, std::vector<float> &outData)
	{
		// first unpack monochromatic bitmap
		std::vector<unsigned char> bitMapData;
		unpack_mono_bitmap(bitmap, bitMapData);

		int width = bitmap->width + 2 * border;
		int height = bitmap->rows + 2 * border;

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
					int idx = (y - border)*(width - 2 * border) + x - border;
					val = bitMapData[idx];
				}

				resizedBitMapData[y*width + x] = val;
			}
		}


		std::vector<glm::vec2> grid1 = std::vector<glm::vec2>(width * height);
		std::vector<glm::vec2> grid2 = std::vector<glm::vec2>(width * height);

		const float INSIDE = 0.0f;
		const float EMPTY  = 100000.0f;

		// init grids
		for (int x = 0; x < width; ++x)
		{
			for (int y = 0; y < height; ++y)
			{
				int idx = y*width + x;
				auto val = resizedBitMapData[idx];

				if (val < 128) {
					grid1[idx] = glm::vec2(INSIDE);
					grid2[idx] = glm::vec2(EMPTY);
				}
				else {
					grid1[idx] = glm::vec2(EMPTY);
					grid2[idx] = glm::vec2(INSIDE);
				}
			}
		}

		// Generate the SDF.
		generate_sdf(grid1, width, height);
		generate_sdf(grid2, width, height);

		float minDist = std::numeric_limits<float>::max();
		float maxDist = std::numeric_limits<float>::lowest();


		float spread = 128.0f;

		std::vector<float> sdf = std::vector<float>(width * height);
		std::vector<bool> processed = std::vector<bool>(width *height);

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				// Calculate the actual distance from the dx/dy
				glm::vec2 val_1 = grid1[y*width + x];
				glm::vec2 val_2 = grid2[y*width + x];
	

				float dist1 = glm::length(val_1);
				float dist2 = glm::length(val_2);

				//std::cout << dist1 << " " << dist2 << std::endl;
				float dist = -1 * (dist1 - dist2);
		
				sdf[y*width + x] = dist;

				if (std::abs(dist) < spread)
				{	
					minDist = std::min(minDist, dist);
					maxDist = std::max(maxDist, dist);
					processed[y*width + x] = true;
				}
				else {
					processed[y*width + x] = false;
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