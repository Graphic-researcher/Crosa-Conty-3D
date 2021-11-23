#pragma once
#include "glm/gtx/string_cast.hpp"
#include "CC3D/Core/Base.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace CC3D {
	class Log
	{
	public:
		static void Init();

		static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};
}


template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternio)
{
	return os << glm::to_string(quaternio);
}

// core log macro
#define CC3D_CORE_TRACE(...)		::CC3D::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CC3D_CORE_INFO(...)			::CC3D::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CC3D_CORE_WARN(...)			::CC3D::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CC3D_CORE_ERROR(...)		::CC3D::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CC3D_CORE_CRITICAL(...)		::CC3D::Log::GetCoreLogger()->critical(__VA_ARGS__) 

// client log macro
#define CC3D_TRACE(...)				::CC3D::Log::GetClientLogger()->trace(__VA_ARGS__)
#define CC3D_INFO(...)				::CC3D::Log::GetClientLogger()->info(__VA_ARGS__)
#define CC3D_WARN(...)				::CC3D::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CC3D_ERROR(...)				::CC3D::Log::GetClientLogger()->error(__VA_ARGS__)
#define CC3D_CRITICAL(...)				::CC3D::Log::GetClientLogger()->critical(__VA_ARGS__) 