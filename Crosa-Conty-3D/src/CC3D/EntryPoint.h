#pragma once

#ifdef  CC3D_PLATFORM_WINDOWS

extern CC3D::Application* CC3D::CreateApplication();

int main(int argc, char** argv)
{
	printf("CC3D!");
	auto app = CC3D::CreateApplication();
	app->Run();
	delete app;
}
#endif //  CC3D_PLATFORM_WINDOWS
