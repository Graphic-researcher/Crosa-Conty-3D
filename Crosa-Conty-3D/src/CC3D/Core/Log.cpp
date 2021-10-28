#include "ccpch.h"

#include "Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace CC3D {
	Ref<spdlog::logger> Log::s_CoreLogger;
	Ref<spdlog::logger>  Log::s_ClientLogger;

	void Log::Init()
	{
		// 通用格式 https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_CoreLogger = spdlog::stdout_color_mt("CC3D");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
	}

}

