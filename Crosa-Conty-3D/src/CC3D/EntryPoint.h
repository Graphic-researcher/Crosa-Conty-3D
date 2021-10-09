#pragma once

#ifdef  CC3D_PLATFORM_WINDOWS

extern CC3D::Application* CC3D::CreateApplication();

int main(int argc, char** argv)
{
	CC3D::Log::Init();
	CC3D_CORE_WARN("Initialized Log");
	CC3D_INFO("Hello Crosa Conty 3D!");
	int a = 5;
	CC3D_INFO("Var = {0}", a);

	auto app = CC3D::CreateApplication();
	app->Run();
	delete app;
}
#endif //  CC3D_PLATFORM_WINDOWS
