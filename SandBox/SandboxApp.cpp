#include <CC3D.h>

class Sandbox : public CC3D::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

CC3D::Application* CC3D::CreateApplication()
{
	return new Sandbox();
}