#pragma once

#ifdef CC3D_PLATFORM_WINDOWS
	#ifdef CC3D_BUILD_DLL
		#define CC3D_API __declspec(dllexport)
	#else 
		#define CC3D_API __declspec(dllimport)
	#endif
#else
	#error CC3D only support Windows!
#endif