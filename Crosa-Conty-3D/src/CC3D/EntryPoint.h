#pragma once

#ifdef  CC3D_PLATFORM_WINDOWS

extern CC3D::Application* CC3D::CreateApplication();

int main(int argc, char** argv)
{
	CC3D::Log::Init();
	CC3D::Log::GetCoreLogger()->warn("Initialized Log");
	CC3D::Log::GetClientlogger()->info("Hello Crosa Conty 3D!");

	auto app = CC3D::CreateApplication();
	app->Run();
	delete app;
}
#endif //  CC3D_PLATFORM_WINDOWS
