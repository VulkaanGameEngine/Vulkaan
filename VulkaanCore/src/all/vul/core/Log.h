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

#define VUL_LOG_CORE_TRACE(...)    ::vul::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define VUL_LOG_CORE_INFO(...)     ::vul::Log::GetCoreLogger()->info(__VA_ARGS__)
#define VUL_LOG_CORE_WARN(...)     ::vul::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define VUL_LOG_CORE_ERROR(...)    ::vul::Log::GetCoreLogger()->error(__VA_ARGS__)
#define VUL_LOG_CORE_CRITICAL(...)    ::vul::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define VUL_LOG_TRACE(...)	      ::vul::Log::GetClientLogger()->trace(__VA_ARGS__)
#define VUL_LOG_INFO(...)	      ::vul::Log::GetClientLogger()->info(__VA_ARGS__)
#define VUL_LOG_WARN(...)	      ::vul::Log::GetClientLogger()->warn(__VA_ARGS__)
#define VUL_LOG_ERROR(...)	      ::vul::Log::GetClientLogger()->error(__VA_ARGS__)
#define VUL_LOG_CRITICAL(...)	      ::vul::Log::GetClientLogger()->critical(__VA_ARGS__)