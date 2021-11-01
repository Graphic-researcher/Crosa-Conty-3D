#include "cc3d_pch.h"
#include "CC3D/Core/Window.h"

#ifdef CC3D_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif

namespace CC3D
{

	Scope<Window> Window::Create(const WindowProps& props)
	{
#ifdef CC3D_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(props);
#else
		CC3D_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}

}