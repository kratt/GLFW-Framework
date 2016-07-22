#include "Log.h"
#include <string>
#include <vector>
#include <map>
#include <chrono>


template<typename Ch, typename Traits = std::char_traits<Ch> >
struct basic_nullbuf : std::basic_streambuf<Ch, Traits> {
	 typedef std::basic_streambuf<Ch, Traits> base_type;
	 typedef typename base_type::int_type int_type;
	 typedef typename base_type::traits_type traits_type;

	 virtual int_type overflow(int_type c) {
		 return traits_type::not_eof(c);
	 }
};

// convenient typedefs
typedef basic_nullbuf<char> nullbuf;
typedef basic_nullbuf<wchar_t> wnullbuf;



/*std::ostream &_LOG_(int line, std::string file) {

	std::cout << line << ": " << file << ": ";


   return std::cout;
}*/



// in a concrete .cpp
nullbuf null_obj;
std::ostream cnull(&null_obj);
//std::wostream wcnull(&wnull_obj);



namespace logger {


	namespace style {
#if 1
		std::string reset  = "\033[0m";
		std::string black  = "\033[0;30m";
		std::string red    = "\033[0;31m";
		std::string green  = "\033[0;32m";
		std::string yellow = "\033[0;33m";
		std::string blue   = "\033[0;34m";
		std::string purple = "\033[0;35m";
		std::string cyan   = "\033[0;36m";
		std::string white  = "\033[0;37m";

		std::string black_bold   = "\033[1;30m";
		std::string red_bold     = "\033[1;31m";
		std::string green_bold   = "\033[1;32m";
		std::string yellow_bold  = "\033[1;33m";
		std::string blue_bold    = "\033[1;34m";
		std::string purple_bold  = "\033[1;35m";
		std::string cyan_bold    = "\033[1;36m";
		std::string white_bold   = "\033[1;37m";
#else
		std::string reset  = "";
		std::string black  = "";
		std::string red    = "";
		std::string green  = "";
		std::string yellow = "";
		std::string blue   = "";
		std::string purple = "";
		std::string cyan   = "";
		std::string white  = "";


		std::string black_bold   = "";
		std::string red_bold     = "";
		std::string green_bold   = "";
		std::string yellow_bold  = "";
		std::string blue_bold    = "";
		std::string purple_bold  = "";
		std::string cyan_bold    = "";
		std::string white_bold   = "";

#endif
	}

	std::map<std::string, size_t> nsIdxMapping;
	std::map<std::string, std::chrono::time_point<std::chrono::system_clock> > timeDiffs;
	std::string lastNS = "default";

	std::string getColor(size_t i) {
		switch(i%9) {
		case 0:
			return style::red;
		case 1:
			return style::green;
		case 2:
			return style::cyan_bold;
		case 3:
			return style::purple;
		case 4:
			return style::cyan;
		case 5:
			return style::red_bold;
		case 6:
			return style::green_bold;
		case 7:
			return style::blue_bold;
		case 8:
			return style::purple_bold;
		case 9:
			return style::blue;
		}
		return "";
	}

	std::string SplitFilename (const std::string& str) {

	  size_t found = str.find_last_of("/\\");

	  std::string tmp =  str.substr(found+1);
	  size_t lastindex = tmp.find_last_of(".");

	  return tmp.substr(0, lastindex);
	}



	std::vector<std::string> split(const std::string &text, char sep) {
	  std::vector<std::string> tokens;
	  size_t start = 0, end = 0;
	  while ((end = text.find(sep, start)) != std::string::npos) {
		tokens.push_back(text.substr(start, end - start));
		start = end + 1;
	  }
	  tokens.push_back(text.substr(start));
	  return tokens;
	}


	std::vector<std::string> logFilter;

	void debugFilterOutput(const std::string &filter) {
		std::cout << style::yellow << "[logger]:" <<  style::reset << " changed filter to '" << filter << "'" << std::endl;
		logFilter =  split(filter,',');
	}


	_time_cmd time_reset() {
		//timeDiffs[lastNS] = std::chrono::system_clock::now();

		return {1};
	}


	_time_cmd time_diff() {
		return {2};
	}


	std::ostream &operator<<(std::ostream &os, _time_cmd const &v) {

		if( v.cmd == 1 ) {
			timeDiffs[lastNS] = std::chrono::system_clock::now();
		} else {
			std::chrono::time_point<std::chrono::system_clock> start, end;
			end   = std::chrono::system_clock::now();
			start = timeDiffs[lastNS];
			std::chrono::duration<double> elapsed_seconds = end-start;
			os << style::yellow << " [" << std::to_string(elapsed_seconds.count()) << "s]" << style::reset;
		}

		return os;
	}



	void _soft_assert(int line, std::string file, bool eq, std::string message) {
		std::cout << style::red << "[assert]: " << message << style::reset << std::endl;
	}


	std::ostream &log(int line, std::string file, std::string ns) {

		if( ns == "default" ) {
			ns = "default:"+SplitFilename(file);
		}

		lastNS = ns;

		if( nsIdxMapping.find(ns) == nsIdxMapping.end() ) {
			nsIdxMapping[ns] = nsIdxMapping.size();
		}



		std::ostream *stream = &std::cout;


		for( auto filter : logFilter ) {
			if( filter.front() == '-' ) {
				if( filter.back() == '*' ) {
					if( ns.substr(0, filter.size()-2) == filter.substr(1,filter.size()-2) ) {
						stream = &cnull;
					}
				} else {
					if( filter.substr(1, filter.size()-1) == ns ) {
						stream = &cnull;
					}
				}
			} else {
				if( filter.back() == '*' ) {
					if( ns.substr(0, filter.size()-1) == filter.substr(0,filter.size()-1) ) {
						stream = &std::cout;
						break;
					}
				} else {
					if( filter == ns ) {
						stream = &std::cout;
						break;
					} else {
						stream = &cnull;
						break;
					}
				}
			}
		}
		/*if( ns.substr(0, prefix.size()) == prefix ) {
			 stream = &cnull;
		}*/

		/*
		 * TODO
		 * if last ns is the same as this ns then we just intend and use a star in the same color but don't write the text
		 */

        (*stream) << getColor(nsIdxMapping[ns]) << "[" << ns << "]" << style::reset << ": ";


		return *stream;

	}
}


