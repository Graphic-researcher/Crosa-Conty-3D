#pragma once

#ifdef  CC3D_PLATFORM_WINDOWS

extern CC3D::Application* CC3D::CreateApplication();

int main(int argc,char** argv)
{
	//Log message
	
	CC3D::Log::Init();
	CC3D_CORE_WARN("Initializing...\n");
	CC3D_INFO("hello! CC3D!");

	auto app = CC3D::CreateApplication();
	app->Run();
	delete app;
	return 0;
}
#else
	#error CC3D only supports Windows!
#endif //  CC3D_PLATFORM_WINDOWS
