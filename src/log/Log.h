//Author: Till Niese
//Date: 10.04.2015

#ifndef LOGGER_LOG_H
#define LOGGER_LOG_H

#include <iostream>


namespace logger {
	std::ostream &log(int line, std::string file, std::string ns="default");
	void _soft_assert(int line, std::string file, bool eq, std::string message);
	void debugFilterOutput(const std::string &filter);


	namespace style {
		extern std::string reset;

		extern std::string black;
		extern std::string red;
		extern std::string green;
		extern std::string yellow;
		extern std::string blue;
		extern std::string purple;
		extern std::string cyan;
		extern std::string white;


		extern std::string black_bold;
		extern std::string red_bold;
		extern std::string green_bold;
		extern std::string yellow_bold;
		extern std::string blue_bold;
		extern std::string purple_bold;
		extern std::string cyan_bold;
		extern std::string white_bold;
	};


	struct _time_cmd {
		int cmd;
	};


	std::ostream &operator<<(std::ostream &os, _time_cmd const &v);

	_time_cmd time_reset();
	_time_cmd time_diff();
}



/*
 *
 * debugLogNs("gestalt:functionName")
 * debugLogNs("name:gestalt:functionName")
 *
 * debugFilterOutput("name:*)
 * debugFilterOutput("name:gestalt:*)
 */

#define soft_assert(eq, message) logger::_soft_assert(__LINE__, __FILE__, eq, message)
#define debugLog() logger::log(__LINE__, __FILE__)
#define debugLogNs(ns) logger::log(__LINE__, __FILE__, ns)

#endif
