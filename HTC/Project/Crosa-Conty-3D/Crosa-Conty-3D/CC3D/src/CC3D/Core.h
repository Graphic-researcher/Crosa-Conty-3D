#pragma once

#ifdef  CC3D_PLATFORM_WINDOWS
	#if CC3D_DYNAMIC_LINK
		#ifdef CC3D_BUILD_DLL
			#define CC3D_API _declspec(dllexport)
		#else
			#define CC3D_API _declspec(dllimport)
	#endif // CC3D_BUILD_DLL
	#else
		#define CC3D_API
	#endif
#else
	#error CC3D only supports Windows!
#endif //  CC3D_PLATFORM_WINDOWS


//for Window
///linking runtime library dynamically; 
///defined CC3D_ENABLE_ASSERTS if CC3D_DEBUG is on.
#ifdef CC3D_DEBUG
#define CC3D_ENABLE_ASSERTS
#endif

///check certain conditons,log message
///if it fails,it would call debugbreak -> set a debug point on that line,to see what's wrong
#ifdef CC3D_ENABLE_ASSERTS
	#define CC3D_ASSERT(x, ...) { if(!(x)) { CC3D_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define CC3D_CORE_ASSERT(x, ...) { if(!(x)) { CC3D_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define CC3D_ASSERT(x, ...)
	#define CC3D_CORE_ASSERT(x, ...)
#endif


//for Event.h
#define BIT(x) (1 << x)


///see more details about this:
///std::bind(&XXX,this,std::placeholders::_1)
///https://stackoverflow.com/questions/61649318/how-could-c-stdbind-return-value-assigned-to-stdfunction
#define CC3D_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)