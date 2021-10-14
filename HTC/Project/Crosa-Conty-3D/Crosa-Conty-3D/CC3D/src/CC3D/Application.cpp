#include "cc3d_pch.h"
#include "Application.h"
#include "CC3D/Log.h"

#include "Input.h"


#include "glad/glad.h"
#include "GLFW/glfw3.h"


namespace CC3D {
#define BIND_EVENT_FN(x) std::bind(&Application::x,this,std::placeholders::_1)

	Application* Application::s_Instance = nullptr;


	Application::Application()
	{
		CC3D_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());///same as below
		///m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	}
	Application::~Application()
	{
	}
	void Application::Run()
	{
		//glfwInit();
		/// <summary>
		/// !!!!very very very important here!!!!
		/// !!!!very very very important here!!!!
		/// !!!!very very very important here!!!!
		/// If have the error of 0X00000 with glad 
		/// The solution is below:
		/// gladLoadGL();
		/// </summary>
		/// refer:
		/// https://stackoverflow.com/questions/67400482/access-violation-executing-location-0x0000000000000000-opengl-with-glad-and-glf
		gladLoadGL();//Load GLAD so it configures OpenGL
		/// <summary>
		/// !!!!very very very important here!!!!
		/// !!!!very very very important here!!!!
		/// !!!!very very very important here!!!!
		/// </summary>
		while (m_Running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();
			auto [x, y] = Input::GetMousePosition();
			CC3D_CORE_TRACE("{0},{1}", x, y);
			auto x1 = Input::IsKeyPressed(GLFW_KEY_A);
			CC3D_CORE_TRACE("{0}", x1);
			m_Window->OnUpdate();
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		CC3D_CORE_INFO("{0}", e);
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

}