//#include "../../CC3D/src/CC3D.h"
#include "CC3D.h"
class ExampleLayer : public CC3D::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
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

class SandBox : public CC3D::Application
{
public:
	SandBox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new CC3D::ImGuiLayer());
	}
	~SandBox() {}

};

CC3D::Application* CC3D::CreateApplication()
{
	return new SandBox();
}