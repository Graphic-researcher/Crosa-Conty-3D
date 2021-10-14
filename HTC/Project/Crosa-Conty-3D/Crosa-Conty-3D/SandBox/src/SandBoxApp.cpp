//#include "../../CC3D/src/CC3D.h"
#include "CC3D.h"


///Test glm include
///glm:https://glm.g-truc.net/0.9.9/
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
glm::mat4 camera(float Translate, glm::vec2 const& Rotate)
{
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.f);
	glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
	View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
	View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
	return Projection * View * Model;
}

class ExampleLayer : public CC3D::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
		auto cam = camera(5.0f, {.5f,.5f });
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
		PushOverlay(new CC3D::ImGuiLayer());
	}
	~SandBox() {}

};

CC3D::Application* CC3D::CreateApplication()
{
	return new SandBox();
}