#pragma once

#ifdef  CC3D_PLATFORM_WINDOWS

extern CC3D::Application* CC3D::CreateApplication();

int main(int argc,char** argv)
{
	auto app = CC3D::CreateApplication();
	app->Run();
	delete app;
	return 0;
}
#else
	#error CC3D only supports Windows!
#endif //  CC3D_PLATFORM_WINDOWS
