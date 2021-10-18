#pragma once

#include <memory>

#ifdef CC3D_PLATFORM_WINDOWS
	#if CC3D_DYNAMIC_LINK
		#ifdef CC3D_BUILD_DLL
			#define CC3D_API __declspec(dllexport)
		#else 
			#define CC3D_API __declspec(dllimport)
		#endif
	#else 
		#define CC3D_API
	#endif
#else
	#error CC3D only support Windows!
#endif

#ifdef CC3D_ENABLE_ASSERTS
		#define CC3D_ASSERT(x, ...) {if(!(x)){CC3D_ERROR("Assertion Failed: {0}", __VA_ARGS__);__debugbreak();} }
		#define CC3D_CORE_ASSERT(x, ...){if(!(x)) {CC3D_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__);__debugbreak;}}
#else
		#define	CC3D_ASSERT(x, ...) 
		#define CC3D_CORE_ASSERT(x, ...) 
#endif

#define BIT(x) (1 << x)

#define CC3D_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace CC3D {
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;
}