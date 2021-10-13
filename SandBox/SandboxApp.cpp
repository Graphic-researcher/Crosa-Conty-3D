#include <CC3D.h>

class ExampleLayer: public CC3D::Layer
{
public:
	ExampleLayer()
		:Layer("Example")
	{
	}

	void OnUpdate() override
	{
		CC3D_INFO("ExampleLayer::Update");
	}

	void OnEvent(CC3D::Event& event) override
	{
		CC3D_TRACE("{0}", event);
	}
};

class Sandbox : public CC3D::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new CC3D::ImGuiLayer());
	}

	~Sandbox()
	{

	}
};

CC3D::Application* CC3D::CreateApplication()
{
	return new Sandbox();
}