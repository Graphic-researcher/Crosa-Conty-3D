#include "cc3d_pch.h"
#include "Application.h"



/////// Event System //////
#include "CC3D/Events/ApplicationEvent.h"
#include "CC3D/Events/KeyEvent.h"
#include "CC3D/Log.h"
///////////////////////////


#include "GLFW/glfw3.h"


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
		while (m_Running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}
}