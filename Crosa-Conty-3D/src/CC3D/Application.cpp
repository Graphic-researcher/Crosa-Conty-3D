#include "ccpch.h"
#include "Application.h"

#include "Events/ApplicationEvent.h"

#include <GLFW/glfw3.h>


namespace CC3D {
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application()
	{
	}
	void Application::Run()
	{

		while (1)
		{
			while (m_Running)
			{
				glClearColor(0.5f, 0.0f, 0.5f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);

				m_Window->OnUpdate();
			}
		}
	}
}

