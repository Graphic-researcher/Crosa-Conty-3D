#pragma once

#ifdef  CC3D_PLATFORM_WINDOWS
	#ifdef CC3D_BUILD_DLL
		#define CC3D_API _declspec(dllexport)
	#else
		#define CC3D_API _declspec(dllimport)
	#endif // CC3D_BUILD_DLL
#else
	#error CC3D only supports Windows!
#endif //  CC3D_PLATFORM_WINDOWS


//for Window
#ifdef CC3D_ENABLE_ASSERTS
	#define CC3D_ASSERT(x, ...) { if(!(x)) { CC3D_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define CC3D_CORE_ASSERT(x, ...) { if(!(x)) { CC3D_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define CC3D_ASSERT(x, ...)
	#define CC3D_CORE_ASSERT(x, ...)
#endif


//for Event.h
#define BIT(x) (1 << x)
