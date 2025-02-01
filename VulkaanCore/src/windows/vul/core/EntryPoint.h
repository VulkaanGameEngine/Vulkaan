#pragma once

#include <vul/core/App.h>
#include <vul/core/Log.h>

extern vul::App* vul::CreateApplication();

int main() {
	vul::Log::Init();

#ifdef VUL_CONF_DEBUG
	VUL_LOG_CORE_TRACE("Test: Trace");
	VUL_LOG_CORE_INFO("Test: Info");
	VUL_LOG_CORE_WARN("Test: Warn");
	VUL_LOG_CORE_ERROR("Test: Error");
	VUL_LOG_CORE_CRITICAL("Test: Critical");
#endif

	vul::App* app = vul::CreateApplication();

	int result = app->Run();

	delete app;

	return result;
}