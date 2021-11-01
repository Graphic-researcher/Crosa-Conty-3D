#include "CC3D.h"
#include <imgui/imgui.h>


//-------Entry Point--------------
#include "CC3D/Core/EntryPoint.h"
//--------------------------------
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"
#include "ExampleLayer.h"


class SandBox : public CC3D::Application
{
public:
	SandBox()
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}
	~SandBox() {}

};

CC3D::Application* CC3D::CreateApplication()
{
	return new SandBox();
}