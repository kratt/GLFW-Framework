#include "opengl_utils.h"
#include "image_utils.h"
#include "system_utils.h"

#include "RenderContext.h"

#include <algorithm>
#include <iostream>
#include <regex> 


namespace utils {


	template<typename T>
	void flip_data(std::vector<T> &data, const unsigned int width, const unsigned int height, bool horizontal, bool vertical)
	{
		unsigned int numComponents = data.size() / (width * height);
		std::vector<T> res = std::vector<T>(data.size());

		for (int y = 0; y < height; ++y){
			for (int x = 0; x < width; ++x){
				for (int c = 0; c < numComponents; ++c) {
					int idx = y*width + x;
					int idx_last = (horizontal ? height - y - 1 : y) * width + (vertical ? width - x - 1 : x);
					res[numComponents * idx + c] = data[numComponents * idx_last + c];
				}			
			}
		}

		std::swap(data, res);
	}

	bool save_framebuffer(const std::string & fileName)
	{
		auto param = RenderContext::globalObjectParam();
		int width = param->windowWidth;
		int height = param->windowHeight;

		auto data = get_framebuffer_data(0, 0, width, height);

		return utils::save_image_rgba(fileName, data, width, height);
	}

	bool save_depthbuffer(const std::string & fileName)
	{
		auto param = RenderContext::globalObjectParam();
		int width = param->windowWidth;
		int height = param->windowHeight;

		auto data = get_depthbuffer_data(0, 0, width, height);

		return utils::save_image_alpha(fileName, data, width, height);
	}

	bool save_texture(const std::string & fileName, GLuint texId, bool saveMipMaps)
	{

		glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
		// valid texture id?
		GLint resident;
		glGetTextureParameteriv(texId, GL_TEXTURE_RESIDENT, &resident);

		if (!resident) {
			std::cout << "save_texture(). Texture with id: " << texId << " does not exist!" << std::endl;
			return false;
		}
			

		GLint max_mipmap = 0;
		std::string fileNameNoExt = std::regex_replace(fileName, std::regex(".png"), "");

		if (saveMipMaps) {
			max_mipmap = get_num_mipmaps(texId);
		}
		
		bool sucess = true;
		for (GLint iLevel = 0; iLevel < max_mipmap + 1; ++iLevel)
		{
			GLint width, height;
			glGetTextureLevelParameteriv(texId, iLevel, GL_TEXTURE_WIDTH, &width);
			glGetTextureLevelParameteriv(texId, iLevel, GL_TEXTURE_HEIGHT, &height);

			//glPixelStorei(GL_PACK_ALIGNMENT, 1);
			std::vector<float> data = std::vector<float>(4 * width * height);
			glGetTextureImage(texId, iLevel, GL_RGBA, GL_FLOAT, width * height * 4 * sizeof(float), data.data());


			std::string finalName = fileName;
			if (iLevel > 0)
				finalName = fileNameNoExt + "_" + std::to_string(iLevel) + ".png";

			sucess &= utils::save_image_rgba(finalName, data, width, height);
		}

		return sucess;
	}

	bool save_texture_bind(const std::string & fileName, GLuint texId, bool saveMipMaps)
	{		
		std::cout << "save_texture_bind(). id: " << texId << std::endl;
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texId);

		GLint width, height;
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0,  GL_TEXTURE_HEIGHT, &height);

		std::cout << "save_texture_bind(). width:" << width << " , height:" << height << std::endl;

		//glPixelStorei(GL_PACK_ALIGNMENT, 1);
		float *texData = new float[width * height * 4];
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, texData);

		std::vector<float> data = std::vector<float>(width * height * 4 );
		for (int x = 0; x < width; ++x)
		{
			for (int y = 0; y < height; ++y)
			{
				int idx = 4*(y*width + x);
				data[idx]   = texData[idx];
				data[idx+1] = texData[idx+1];
				data[idx+2] = texData[idx+2];
				data[idx+3] = texData[idx+3];
			}
		}

		utils::save_image_rgba(fileName, data, width, height);




		//glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
		//glBindTexture(GL_TEXTURE_2D, texId);


		//GLint max_mipmap = 0;
		//std::string fileNameNoExt = std::regex_replace(fileName, std::regex(".png"), "");

		//if (saveMipMaps) {
		//	max_mipmap = get_num_mipmaps(texId);
		//}

		//bool sucess = true;
		//for (GLint iLevel = 0; iLevel < max_mipmap + 1; ++iLevel)
		//{
		//	GLint width, height;
		//	glGetTexLevelParameteriv(GL_TEXTURE_2D, iLevel, GL_TEXTURE_WIDTH, &width);
		//	glGetTexLevelParameteriv(GL_TEXTURE_2D, iLevel, GL_TEXTURE_HEIGHT, &height);

		//	//glPixelStorei(GL_PACK_ALIGNMENT, 1);
		//	std::vector<float> data = std::vector<float>(4 * width * height);
		//	glGetTexImage(GL_TEXTURE_2D, iLevel, GL_RGBA, GL_FLOAT, data.data());


		//	std::string finalName = fileName;
		//	if (iLevel > 0)
		//		finalName = fileNameNoExt + "_" + std::to_string(iLevel) + ".png";

		//	sucess &= utils::save_image_rgba(finalName, data, width, height);
		//}

		return true;
	}

	std::vector<unsigned char> get_framebuffer_data(const GLint x, const GLint y, const GLsizei width, const GLsizei height)
	{
		std::vector<unsigned char> data = std::vector<unsigned char>(4 * width * height);
		glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data.data());

		flip_data(data, width, height, true, false);
		return data;
	}


	std::vector<float> get_depthbuffer_data(const GLint x, const GLint y, const GLsizei width, const GLsizei height)
	{
		auto data = std::vector<float>(width * height);
		glReadPixels(x, y, width, height, GL_DEPTH_COMPONENT, GL_FLOAT, data.data());

		flip_data(data, width, height, true, false);
		return data;
	}

	GLint get_num_mipmaps(GLint texId)
	{
		GLint resident;
		glGetTextureParameteriv(texId, GL_TEXTURE_RESIDENT, &resident);

		if (!resident)
			return 0;

		GLint max_level;
		glGetTextureParameteriv(texId, GL_TEXTURE_MAX_LEVEL, &max_level);

		GLint max_mipmap = 0;
		for (int i = 0; i < max_level; ++i)
		{
			int tmpWidth;
			glGetTextureLevelParameteriv(texId, i, GL_TEXTURE_WIDTH, &tmpWidth);
			if (0 == tmpWidth)
			{
				max_mipmap = i - 1;
				break;
			}
		}

		return max_mipmap;
	}

	std::vector<GLuint> create_debug_textures()
	{
		std::vector<GLuint> ids;

		/* Create an empty texture that will be used to hold the entire text */
		int maxWidth = 30;
		int maxHeight = 10;

		GLuint texId;
		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, maxWidth, maxHeight, 0, GL_RED, GL_FLOAT, 0);


		std::vector<float> data = std::vector<float>(maxWidth * maxHeight, 1);

		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, maxWidth, maxHeight, GL_RED, GL_FLOAT, data.data());

		ids.push_back(texId);
		return ids;

		int numParts = 3;
		int smallWidth = 9;
		int smallHeight = 10;

		int curPosX = 0;
		int curPosY = 0;
		for (int i = 0; i < numParts; ++i)
		{
			float w = smallWidth;
			float h = smallHeight;

			std::vector<float> data = std::vector<float>(w * h * 4);
			glTexSubImage2D(GL_TEXTURE_2D, 0, curPosX, curPosY, w, h, GL_RED, GL_FLOAT, data.data());

			curPosX += w + 1;
			//glGenerateMipmap(GL_TEXTURE_2D);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		return ids;
	}

}

