#pragma once

#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"


namespace CC3D {
	class CC3D_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

// core log macro
#define CC3D_CORE_TRACE(...)		::CC3D::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CC3D_CORE_INFO(...)			::CC3D::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CC3D_CORE_WARN(...)			::CC3D::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CC3D_CORE_ERROR(...)		::CC3D::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CC3D_CORE_FATAL(...)		::CC3D::Log::GetCoreLogger()->fatal(__VA_ARGS__) 

// client log macro
#define CC3D_TRACE(...)				::CC3D::Log::GetClientLogger()->trace(__VA_ARGS__)
#define CC3D_INFO(...)				::CC3D::Log::GetClientLogger()->info(__VA_ARGS__)
#define CC3D_WARN(...)				::CC3D::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CC3D_ERROR(...)				::CC3D::Log::GetClientLogger()->error(__VA_ARGS__)
#define CC3D_FATAL(...)				::CC3D::Log::GetClientLogger()->fatal(__VA_ARGS__) 