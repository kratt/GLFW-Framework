#include "image_loader.h"

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
}

