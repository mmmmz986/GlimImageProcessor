#include "Log.h"

namespace ip {
	void Log::log(const std::string& message, bool isSuccess, const std::string& params)
	{
		std::stringstream ss;

		// 로그 포맷
		ss << "Log: ";

		// 현재 시간
		std::time_t now = std::time(nullptr);
		std::tm* localTime = std::localtime(&now);

		ss << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");

		// 성공 여부
		if (isSuccess) {
			ss << " | SUCCESS";
		}
		else {
			ss << " | FAIL";
		}

		// 기본	메시지
		ss << " | " << message;

		// 추가 파라미터
		if(!params.empty()) {
			ss << " | Params: " << params;
		}

		// 로그 출력
		std::string logMessage = ss.str();
		std::cout << logMessage << std::endl;
	}
}