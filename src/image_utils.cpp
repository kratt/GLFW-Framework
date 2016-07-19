#include "image_utils.h"

#include <array>
#include <fstream>
#include <iostream>
#include <algorithm>

#include <png.h>
#include <experimental/filesystem>


namespace utils {

	/*
	 std::shared_ptr<Shader> load_shader(const std::string &path) {
	 std::lock_guard<std::recursive_mutex> lock(s_shaderCacheMutex);

	 auto absolutePath = utils::fs::absolute(path).string();
	 std::shared_ptr<Shader> shader;

	 if (s_shaderCache.find(absolutePath) == s_shaderCache.end() ||
	 s_shaderCache[absolutePath].expired()) {
	 shader = std::make_shared<Shader>(absolutePath);
	 s_shaderCache[absolutePath] = shader;

	 } else {
	 shader = s_shaderCache[absolutePath].lock();
	 }

	 return shader;
	 }

	 Shader::Type get_shader_type_from_path(const std::string &path) {
	 std::string ext = utils::fs::extension(path);

	 */

	void ReadDataFromInputStream(png_structp png_ptr, png_bytep outBytes,
		png_size_t byteCountToRead) {
		png_voidp io_ptr = png_get_io_ptr(png_ptr);
		if (io_ptr == NULL) return;  // add custom error handling here

		std::ifstream &stream = *((std::ifstream *)io_ptr);

		stream.read((char *)outBytes, byteCountToRead);

		if ((png_size_t)stream.gcount() != byteCountToRead) return;
	}

#define USE_CPP_STREAM 1

	bool load_png(const std::string &path, std::vector<unsigned char> &data,
		GLenum &type, unsigned int &width, unsigned int &height,
		unsigned int alignment) {
		std::ifstream file(path, std::ios::binary);

		// Stop eating new lines in binary mode!!!
		file.unsetf(std::ios::skipws);

		std::array<png_byte, 8> header;
		file.read((char *)(&header[0]), header.size());

		if (png_sig_cmp(&header[0], 0, 8)) {
			throw std::runtime_error("file doe snot contain png data");
		}

		png_structp png_ptr =
			png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
		if (!png_ptr) {
			throw std::runtime_error("png_create_read_struct returned 0");
		}

		// create png info struct
		png_infop info_ptr = png_create_info_struct(png_ptr);
		if (!info_ptr) {
			png_destroy_read_struct(&png_ptr, (png_infopp) nullptr,
				(png_infopp) nullptr);

			throw std::runtime_error("png_create_info_struct returned 0");
		}

		// create png info struct
		png_infop end_info = png_create_info_struct(png_ptr);
		if (!end_info) {
			png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) nullptr);
			throw std::runtime_error("png_create_info_struct returned 0");
		}

		// the code in this if statement gets called if libpng encounters an error
		if (setjmp(png_jmpbuf(png_ptr))) {
			png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
			throw std::runtime_error("error from libpng");
		}

		png_set_read_fn(png_ptr, &file, ReadDataFromInputStream);
		png_set_sig_bytes(png_ptr, 8);

		png_read_png(
			png_ptr, info_ptr,
			PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND,
			NULL);
		// png_read_info(png_ptr, info_ptr);

		/*png_uint_32 width;
		png_uint_32 height;*/
		int color_type;
		int bit_depth;
		png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
			nullptr, nullptr, nullptr);

		unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);

		int channels = 0;
		if (color_type == PNG_COLOR_TYPE_RGB) {
			type = GL_RGB;
			channels = 3;
		}
		else if (color_type == PNG_COLOR_TYPE_RGBA) {
			type = GL_RGBA;
			channels = 4;
		}

		// TODO check if this is correct or glGetIntegerv(GL_PACK_ALIGNMENT,
		// &alignment) should be used
		// https://www.opengl.org/wiki/Pixel_Transfer
		unsigned short alignmentFix = 3 - ((row_bytes - 1) % alignment);

		data.reserve(width * height);

		png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
		std::vector<png_bytep> rowpointers(row_pointers, row_pointers + height);

		for (png_uint_32 i = 0; i < height; i++) {
			unsigned int srcIdx = height - 1 - i;
			auto startPtr = row_pointers[srcIdx];
			auto endPtr = row_pointers[srcIdx] + (width * channels);

			std::copy(startPtr, endPtr, std::back_inserter(data));
			data.insert(data.end(), alignmentFix, 0);  // correct alignment
		}

		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

		return true;
	}

	bool load_image(const std::string &path, std::vector<unsigned char> &data,
		GLenum &type, unsigned int &width, unsigned int &height,
		unsigned int alignment) 
	{
		auto fi = std::experimental::filesystem::path(path);

		std::string ext = fi.extension().string();

		//auto absolutePath = utils::fs::absolute(path).string();
		//std::string ext = utils::fs::extension(path);

		if (ext == ".png") {
			//std::cout << "load png" << std::endl;
			return load_png(path, data, type, width, height, alignment);
		}
		else {
			std::cout << "unknown extension" << std::endl;
			return false;
		}
	}

	bool save_image(const std::string & path,  const std::vector<unsigned char>& data, unsigned int width, unsigned int height)
	{
		FILE *fp = nullptr;
		png_structp png_ptr = nullptr;
		png_infop info_ptr = nullptr;
		png_bytep row = nullptr;

		// Open file for writing (binary mode)
		fp = fopen(path.c_str(), "wb");
		if (fp == nullptr) {
			fprintf(stderr, "Could not open file %s for writing\n", path);
			return false;
		}

		// Initialize write structure
		png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (png_ptr == nullptr) {
			fprintf(stderr, "Could not allocate write struct\n");
			return false;
		}

		// Initialize info structure
		info_ptr = png_create_info_struct(png_ptr);
		if (info_ptr == nullptr) {
			fprintf(stderr, "Could not allocate info struct\n");
			return false;
		}

		// Setup Exception handling
		if (setjmp(png_jmpbuf(png_ptr))) {
			fprintf(stderr, "Error during png creation\n");
			return false;
		}

		png_init_io(png_ptr, fp);

		// Write header (8 bit colour depth)
		png_set_IHDR(png_ptr, info_ptr, width, height,
			8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

		png_write_info(png_ptr, info_ptr);

		// Allocate memory for one row (4 bytes per pixel - RGBA)
		row = (png_bytep)malloc(4 * width * sizeof(png_byte));
		// Write image data
		for (int y = 0; y<height; y++) 
		{
			for (int x = 0; x<width; x++) 
			{
				int idx = y*width + x;
				auto prt = &(row[x * 4]);
				prt[0] = data[4 * idx];
				prt[1] = data[4 * idx + 1];
				prt[2] = data[4 * idx + 2];
				prt[3] = data[4 * idx + 3];
			}
			png_write_row(png_ptr, row);
		}

		// End write
		png_write_end(png_ptr, NULL);

		
		if (fp) fclose(fp);
		if (info_ptr) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
		if (png_ptr) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		if (row) free(row);

		return true;
	}

	bool save_image_rgb(const std::string & path, const std::vector<float>& data, unsigned int width, unsigned int height, float minVal, float maxVal)
	{
		// check if we have enough data
		if (width * height * 3 != data.size()) {
			std::cout << "save_image_rgb(). Data size does not match color type!" << std::endl;
			return false;
		}

		// remap data
		std::vector<unsigned char> imgData = std::vector<unsigned char>(width * height * 4);

		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				int idx = y*width + x;

				imgData[4 * idx]     = (unsigned char)((data[3 * idx]     - minVal) / (maxVal - minVal) * 255);
				imgData[4 * idx + 1] = (unsigned char)((data[3 * idx + 1] - minVal) / (maxVal - minVal) * 255);
				imgData[4 * idx + 2] = (unsigned char)((data[3 * idx + 2] - minVal) / (maxVal - minVal) * 255);
				imgData[4 * idx + 3] = 255;
			}
		}

		return save_image(path, imgData, width, height);
	}

	bool save_image_rgba(const std::string & path, const std::vector<float>& data, unsigned int width, unsigned int height, float minVal, float maxVal)
	{
		// check if we have enough data
		if (width * height * 4 != data.size()) {
			std::cout << "save_image_rgba(). Data size does not match color type!" << std::endl;
			return false;
		}

		// remap data to rgba
		std::vector<unsigned char> imgData = std::vector<unsigned char>(width * height * 4);

		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				int idx = y*width + x;

				imgData[4 * idx]     = (unsigned char)((data[4 * idx]     - minVal) / (maxVal - minVal) * 255);
				imgData[4 * idx + 1] = (unsigned char)((data[4 * idx + 1] - minVal) / (maxVal - minVal) * 255);
				imgData[4 * idx + 2] = (unsigned char)((data[4 * idx + 2] - minVal) / (maxVal - minVal) * 255);
				imgData[4 * idx + 3] = (unsigned char)((data[4 * idx + 3] - minVal) / (maxVal - minVal) * 255);
			}
		}

		return save_image(path, imgData, width, height);
	}

	bool save_image_alpha(const std::string & path, const std::vector<float>& data, unsigned int width, unsigned int height, float minVal, float maxVal)
	{
		// check if we have enough data
		if (width * height != data.size()) {
			std::cout << "save_image_alpha(). Data size does not match color type!" << std::endl;
			return false;
		}

		// remap data to rgba
		std::vector<unsigned char> imgData = std::vector<unsigned char>(width * height * 4);

		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				int idx = y*width + x;

				auto val = (unsigned char)((data[idx] - minVal) / (maxVal - minVal) * 255);

				imgData[4 * idx]     = val;
				imgData[4 * idx + 1] = val;
				imgData[4 * idx + 2] = val;
				imgData[4 * idx + 3] = 255;
			}
		}

		return save_image(path, imgData, width, height);
	}


	bool save_image_rgb(const std::string &path, const std::vector<unsigned char> &data, unsigned int width, unsigned int height, unsigned char minVal, unsigned char maxVal)
	{
		// check if we have enough data
		if (width * height * 3 != data.size()) {
			std::cout << "save_image_rgb(). Data size does not match color type!" << std::endl;
			return false;
		}

		// remap data
		std::vector<unsigned char> imgData = std::vector<unsigned char>(width * height * 4);

		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				int idx = y*width + x;

				imgData[4 * idx]     = (data[3 * idx]     - minVal) / (maxVal - minVal) * 255;
				imgData[4 * idx + 1] = (data[3 * idx + 1] - minVal) / (maxVal - minVal) * 255;
				imgData[4 * idx + 2] = (data[3 * idx + 2] - minVal) / (maxVal - minVal) * 255;
				imgData[4 * idx + 3] = 255;
			}
		}

		return save_image(path, imgData, width, height);
	}


	bool save_image_rgba(const std::string &path, const std::vector<unsigned char> &data, unsigned int width, unsigned int height, unsigned char minVal, unsigned char maxVal)
	{
		// check if we have enough data
		if (width * height * 4 != data.size()) {
			std::cout << "save_image_rgba(). Data size does not match color type!" << std::endl;
			return false;
		}

		// remap data 
		std::vector<unsigned char> imgData = std::vector<unsigned char>(width * height * 4);

		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				int idx = y*width + x;

				imgData[4 * idx]     = (data[4 * idx]     - minVal) / (maxVal - minVal) * 255;
				imgData[4 * idx + 1] = (data[4 * idx + 1] - minVal) / (maxVal - minVal) * 255;
				imgData[4 * idx + 2] = (data[4 * idx + 2] - minVal) / (maxVal - minVal) * 255;
				imgData[4 * idx + 3] = (data[4 * idx + 3] - minVal) / (maxVal - minVal) * 255;
			}
		}

		return save_image(path, data, width, height);
	}


	bool save_image_alpha(const std::string &path, const std::vector<unsigned char> &data, unsigned int width, unsigned int height, unsigned char minVal, unsigned char maxVal)
	{
		// check if we have enough data
		if (width * height != data.size()) {
			std::cout << "save_image_alpha(). Data size does not match color type!" << std::endl;
			return false;
		}

		// remap data to rgba
		std::vector<unsigned char> imgData = std::vector<unsigned char>(width * height * 4);

		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				int idx = y*width + x;

				auto val = (data[idx] - minVal) / (maxVal - minVal) * 255;

				imgData[4 * idx]     = val;
				imgData[4 * idx + 1] = val;
				imgData[4 * idx + 2] = val;
				imgData[4 * idx + 3] = 255;
			}
		}

		return save_image(path, imgData, width, height);
	}
}

