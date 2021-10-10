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

//for Event.h
#define BIT(x) (1 << x)
