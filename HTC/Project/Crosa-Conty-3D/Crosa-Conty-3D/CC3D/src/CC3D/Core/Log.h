#pragma once
#include "CC3D/Core/Core.h"
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
namespace CC3D {
	class Log
	{
	public:
		static void Init();

		inline static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};
}

//Core log macros
#define CC3D_CORE_TRACE(...)	::CC3D::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CC3D_CORE_INFO(...)		::CC3D::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CC3D_CORE_WARN(...)		::CC3D::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CC3D_CORE_ERROR(...)	::CC3D::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CC3D_CORE_CRITICAL(...) ::CC3D::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define CC3D_TRACE(...)         ::CC3D::Log::GetClientLogger()->trace(__VA_ARGS__)
#define CC3D_INFO(...)          ::CC3D::Log::GetClientLogger()->info(__VA_ARGS__)
#define CC3D_WARN(...)          ::CC3D::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CC3D_ERROR(...)         ::CC3D::Log::GetClientLogger()->error(__VA_ARGS__)
#define CC3D_CRITICAL(...)      ::CC3D::Log::GetClientLogger()->critical(__VA_ARGS__)


