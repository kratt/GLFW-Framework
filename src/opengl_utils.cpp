#include "opengl_utils.h"
#include "image_utils.h"
#include "system_utils.h"

#include "RenderContext.h"

namespace utils {

	bool save_framebuffer(const std::string & path)
	{
		int components = 4;
		int width = 1280;
		int height = 720;


		std::vector<unsigned char> data = std::vector<unsigned char>(components * width * height);
		glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
		
		return utils::save_image_rgba(path, data, width, height);
	}

}

