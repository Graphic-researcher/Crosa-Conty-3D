#pragma once
#include "CC3D/Core/Core.h"

#ifdef  CC3D_PLATFORM_WINDOWS

extern CC3D::Application* CC3D::CreateApplication();

int main(int argc, char** argv)
{
	CC3D::Log::Init();
	CC3D_CORE_WARN("Initialized Log");
	CC3D_INFO("Hello Crosa Conty 3D!");

	CC3D_PROFILE_BEGIN_SESSION("Startup", "CC3DProfile-Startup.json");
	auto app = CC3D::CreateApplication();
	CC3D_PROFILE_END_SESSION();

	CC3D_PROFILE_BEGIN_SESSION("Runtime", "CC3DProfile-Runtime.json");
	app->Run();
	CC3D_PROFILE_END_SESSION();

	CC3D_PROFILE_BEGIN_SESSION("Startup", "CC3DProfile-Shutdown.json");
	delete app;
	CC3D_PROFILE_END_SESSION();
}
#endif //  CC3D_PLATFORM_WINDOWS
