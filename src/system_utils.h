#ifndef SYSTEM_UTILS_H
#define SYSTEM_UTILS_H

#include "Global.h"
#include <string>
#include <vector>


namespace utils 
{
	std::string time_string();

	std::vector<std::string> get_file_list(const std::string &path, const std::string &extension);
}


#endif
