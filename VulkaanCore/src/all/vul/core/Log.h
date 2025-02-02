#pragma once

#define FMT_UNICODE 0
#include <spdlog/spdlog.h>

namespace vul {
	class Log {
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

#define VUL_LOG_CORE_TRACE(...)
#define VUL_LOG_CORE_INFO(...)
#define VUL_LOG_CORE_WARN(...)
#define VUL_LOG_CORE_ERROR(...)
#define VUL_LOG_CORE_CRITICAL(...)

//#define VUL_LOG_CORE_TRACE(...)			::vul::Log::GetCoreLogger()->trace(__VA_ARGS__)
//#define VUL_LOG_CORE_INFO(...)			::vul::Log::GetCoreLogger()->info(__VA_ARGS__)
//#define VUL_LOG_CORE_WARN(...)			::vul::Log::GetCoreLogger()->warn(__VA_ARGS__)
//#define VUL_LOG_CORE_ERROR(...)			::vul::Log::GetCoreLogger()->error(__VA_ARGS__)
//#define VUL_LOG_CORE_CRITICAL(...)		::vul::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define VUL_LOG_TRACE(...)				::vul::Log::GetClientLogger()->trace(__VA_ARGS__)
#define VUL_LOG_INFO(...)				::vul::Log::GetClientLogger()->info(__VA_ARGS__)
#define VUL_LOG_WARN(...)				::vul::Log::GetClientLogger()->warn(__VA_ARGS__)
#define VUL_LOG_ERROR(...)				::vul::Log::GetClientLogger()->error(__VA_ARGS__)
#define VUL_LOG_CRITICAL(...)			::vul::Log::GetClientLogger()->critical(__VA_ARGS__)

#ifdef VUL_CONF_NDIST
// Enable asserts
#define VUL_CORE_ASSERT(condition, msg) if (!(condition)) { VUL_LOG_CORE_CRITICAL(msg); exit(1); }
#define VUL_ASSERT(condition, msg)		if (!(condition)) { VUL_LOG_CRITICAL(msg); exit(1); }
#else
// ignore asserts
#define VUL_CORE_ASSERT(condition, msg)
#define VUL_ASSERT(condition, msg)
#endif