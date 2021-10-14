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
		if (CC3D::Input::IsKeyPressed(CC3D_KEY_TAB))
			CC3D_TRACE("Tab key is pressed (poll)!");
	}

	void OnEvent(CC3D::Event& event) override
	{
		if (event.GetEventType() == CC3D::EventType::KeyPressed)
		{
			CC3D::KeyPressedEvent& e = (CC3D::KeyPressedEvent&)event;
			if (e.GetKeyCode() == CC3D_KEY_TAB)
				CC3D_TRACE("Tab key is pressed (event)!");
			CC3D_TRACE("{0}", (char)e.GetKeyCode());
		}
	}

};

class SandBox : public CC3D::Application
{
public:
	SandBox()
	{
		PushLayer(new ExampleLayer());
	}
	~SandBox() {}

};

CC3D::Application* CC3D::CreateApplication()
{
	return new SandBox();
}