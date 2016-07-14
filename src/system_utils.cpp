#include "system_utils.h"

#include <ctime>

namespace utils {

	std::string time_string()
	{
		time_t t = time(0);   // get time now
		struct tm * now = localtime(&t);

		int year = now->tm_year - 100;
		int month = now->tm_mon + 1;
		int day = now->tm_mday;

		int hour = now->tm_hour;
		int minute = now->tm_min;
		int second = now->tm_sec;

		std::string sYear = std::to_string(year);
		std::string sMonth = std::to_string(month);
		std::string sDay = std::to_string(day);
		std::string sHour = std::to_string(hour);
		std::string sMinute = std::to_string(minute);
		std::string sSecond = std::to_string(second);
		std::string sNull = std::to_string(0);

		std::string  fMonth = month < 10 ? sNull + sMonth : sMonth;
		std::string  fDay = day < 10 ? sNull + sDay : sDay;
		std::string  fHour = hour < 10 ? sNull + sHour : sHour;
		std::string  fMinute = minute < 10 ? sNull + sMinute : sMinute;
		std::string  fSecond = second < 10 ? sNull + sSecond : sSecond;


		return sYear + fMonth + fDay + "_" + fHour + fMinute + fSecond;
	}
}

