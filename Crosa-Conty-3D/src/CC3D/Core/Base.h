#pragma once

#include <memory>
#include "CC3D/Core/PlatformDetection.h"



#ifdef CC3D_DEBUG
#if defined(CC3D_PLATFORM_WINDOWS)
#define CC3D_DEBUGBREAK() __debugbreak()
#elif defined(CC3D_PLATFORM_LINUX)
#include <signal.h>
#define CC3D_DEBUGBREAK() raise(SIGTRAP)
#else
#error "Platform doesn't support debugbreak yet!"
#endif
#define CC3D_ENABLE_ASSERTS
#else
#define CC3D_DEBUGBREAK()
#endif

#ifdef CC3D_ENABLE_ASSERTS
#define CC3D_ASSERT(x, ...) { if(!(x)) { CC3D_ERROR("Assertion Failed: {0}", __VA_ARGS__); CC3D_DEBUGBREAK(); } }
#define CC3D_CORE_ASSERT(x, ...) { if(!(x)) { CC3D_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); CC3D_DEBUGBREAK(); } }
#else
		#define	CC3D_ASSERT(x, ...) 
		#define CC3D_CORE_ASSERT(x, ...) 
#endif

#define BIT(x) (1 << x)

#define CC3D_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace CC3D {
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)// Scope<T> equals to std::unique_ptr<T>
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}