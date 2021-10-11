#include "ccpch.h"
#include "Application.h"

#include "Events/ApplicationEvent.h"

#include <GLFW/glfw3.h>


namespace CC3D {

	// 非静态成员函数需要传递this指针作为第一个参数
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
		//
	}

	Application::~Application()
	{
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		CC3D_CORE_TRACE(" {0}", e);
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

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}

