#include "ccpch.h"
#include "CC3D/Core/Input.h"

#ifdef CC3D_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsInput.h"
#endif

namespace CC3D {
	Scope<Input> Input::s_Instance = Input::Create();

	Scope<Input> Input::Create()
	{
#ifdef CC3D_PLATFORM_WINDOWS
		return CreateScope<WindowsInput>();
#else
		CC3D_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}
}