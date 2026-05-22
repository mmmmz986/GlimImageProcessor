#pragma once
#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>


namespace ip {
	class Log {
	public:
		static void log(const std::string& message, bool isSuccess, const std::string& params = "");
		};
}