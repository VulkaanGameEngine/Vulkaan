#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace vul {
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init() {
#ifdef VUL_CONF_DEBUG
		// Debug
		spdlog::level::level_enum level = spdlog::level::trace;
#else
#ifdef VUL_CONF_RELEASE
		// Release
		spdlog::level::level_enum level = spdlog::level::warn;
#else
		// Dist
		spdlog::level::level_enum level = spdlog::level::err;
#endif
#endif
		spdlog::set_pattern("%^[%T] %n: %v%$");
		
		s_CoreLogger = spdlog::stdout_color_mt("VULKAAN");
		s_CoreLogger->set_level(level);

		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(level);
	}
}