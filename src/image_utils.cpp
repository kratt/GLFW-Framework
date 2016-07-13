#include "image_utils.h"

#include <array>
#include <fstream>
#include <iostream>

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
			std::cout << "load png" << std::endl;
			return load_png(path, data, type, width, height, alignment);
		}
		else {
			std::cout << "unknown extension" << std::endl;
			return false;
		}
	}


	void setRGB(png_byte *ptr, float val)
	{
		int v = (int)(val * 767);
		if (v < 0) v = 0;
		if (v > 767) v = 767;
		int offset = v % 256;

		if (v<256) {
			ptr[0] = 0; ptr[1] = 0; ptr[2] = offset;
		}
		else if (v<512) {
			ptr[0] = 0; ptr[1] = offset; ptr[2] = 255 - offset;
		}
		else {
			ptr[0] = offset; ptr[1] = 255 - offset; ptr[2] = 0;
		}
	}



	bool save_image(const std::string & path, std::vector<unsigned char>& data, unsigned int width, unsigned int height)
	{
		int code = 0;
		FILE *fp = nullptr;
		png_structp png_ptr = nullptr;
		png_infop info_ptr = nullptr;
		png_bytep row = nullptr;

		// Open file for writing (binary mode)
		fp = fopen(path.c_str(), "wb");
		if (fp == NULL) {
			fprintf(stderr, "Could not open file %s for writing\n", path);
			code = 1;
		}

		// Initialize write structure
		png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (png_ptr == NULL) {
			fprintf(stderr, "Could not allocate write struct\n");
			code = 1;
		}

		// Initialize info structure
		info_ptr = png_create_info_struct(png_ptr);
		if (info_ptr == NULL) {
			fprintf(stderr, "Could not allocate info struct\n");
			code = 1;
		}

		// Setup Exception handling
		if (setjmp(png_jmpbuf(png_ptr))) {
			fprintf(stderr, "Error during png creation\n");
			code = 1;
		}

		png_init_io(png_ptr, fp);

		// Write header (8 bit colour depth)
		png_set_IHDR(png_ptr, info_ptr, width, height,
			8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);


		png_write_info(png_ptr, info_ptr);

		// Allocate memory for one row (3 bytes per pixel - RGB)
		row = (png_bytep)malloc(3 * width * sizeof(png_byte));

		// Write image data
		int x, y;
		for (y = 0; y<height; y++) {
			for (x = 0; x<width; x++) {
				setRGB(&(row[x * 3]), data[y*width + x]);
			}
			png_write_row(png_ptr, row);
		}

		// End write
		png_write_end(png_ptr, NULL);


		if (fp != NULL) fclose(fp);
		if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
		if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		if (row != NULL) free(row);

		return code;

		return true;
	}

	int writeImage(char* filename, int width, int height, float *buffer, char* title)
	{
		int code = 0;
		FILE *fp = NULL;
		png_structp png_ptr = NULL;
		png_infop info_ptr = NULL;
		//png_bytep row = NULL;

		// Open file for writing (binary mode)
		fp = fopen(filename, "wb");
		if (fp == NULL) {
			fprintf(stderr, "Could not open file %s for writing\n", filename);
			code = 1;
		}

		// Initialize write structure
		png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (png_ptr == NULL) {
			fprintf(stderr, "Could not allocate write struct\n");
			code = 1;
		}

		// Initialize info structure
		info_ptr = png_create_info_struct(png_ptr);
		if (info_ptr == NULL) {
			fprintf(stderr, "Could not allocate info struct\n");
			code = 1;
		}

		// Setup Exception handling
		if (setjmp(png_jmpbuf(png_ptr))) {
			fprintf(stderr, "Error during png creation\n");
			code = 1;
		}

		png_init_io(png_ptr, fp);

		// Write header (8 bit colour depth)
		png_set_IHDR(png_ptr, info_ptr, width, height,
			8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

		// Set title
		if (title != NULL) {
			png_text title_text;
			title_text.compression = PNG_TEXT_COMPRESSION_NONE;
			title_text.key = "Title";
			title_text.text = title;
			png_set_text(png_ptr, info_ptr, &title_text, 1);
		}

		png_write_info(png_ptr, info_ptr);

		// Allocate memory for one row (3 bytes per pixel - RGB)

		std::vector<png_byte> row = std::vector<png_byte>(3 * width);
		//row = (png_bytep)malloc(3 * width * sizeof(png_byte));

		// Write image data
		int x, y;
		for (y = 0; y<height; y++) {
			for (x = 0; x<width; x++) {

				auto tmpPtr = &(row[x * 3]);
				tmpPtr[0] = 255;
				tmpPtr[1] = 0;
				tmpPtr[1] = 0;

				//setRGB(&(row[x * 3]), buffer[y*width + x]);
			}
			png_write_row(png_ptr, row.data());
		}


		png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);


		// End write
		png_write_end(png_ptr, NULL);


		if (fp != NULL) fclose(fp);
		if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
		if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);


		return code;
	}
}

