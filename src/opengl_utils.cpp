#include "opengl_utils.h"
#include "image_utils.h"
#include "system_utils.h"

#include "RenderContext.h"

#include <algorithm>


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

	bool save_framebuffer(const std::string & path)
	{
		auto param = RenderContext::globalObjectParam();
		int width = param->windowWidth;
		int height = param->windowHeight;

		auto data = get_framebuffer_data(0, 0, width, height);

		std::string fileName = path + utils::time_string() + ".png";
		return utils::save_image_rgba(fileName, data, width, height);
	}


	bool save_depthbuffer(const std::string & path)
	{
		auto param = RenderContext::globalObjectParam();
		int width = param->windowWidth;
		int height = param->windowHeight;

		auto data = get_depthbuffer_data(0, 0, width, height);

		std::string fileName = path + utils::time_string() + ".png";
		return utils::save_image_alpha(fileName, data, width, height);
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

}

