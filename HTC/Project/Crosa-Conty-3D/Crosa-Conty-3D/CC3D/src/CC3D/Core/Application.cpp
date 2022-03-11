#include "cc3d_pch.h"
#include "CC3D/Core/Application.h"
#include "CC3D/Core/Log.h"
#include "CC3D/Core/Input.h"
#include "CC3D/Renderer/Renderer.h"
///temporarily
#include<GLFW/glfw3.h>


namespace CC3D {
	Application* Application::s_Instance = nullptr;


	Application::Application(const std::string& name)
	{
		CC3D_PROFILE_FUNCTION();

		CC3D_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = Window::Create(WindowProps(name));
		m_Window->SetEventCallback(CC3D_BIND_EVENT_FN(Application::OnEvent));
		//m_Window->SetVSync(false);///glfwinterval(0)
		Renderer::Init();
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		CC3D_PROFILE_FUNCTION();

		Renderer::Shutdown();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::Run()
	{
		CC3D_PROFILE_FUNCTION();

		while (m_Running)
		{
			CC3D_PROFILE_SCOPE("RunLoop");

			///time step
			float time = (float)glfwGetTime(); ///Platform : GetTime()
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			//m_Window->OnUpdate();

			if (!m_Minimized)
			{
				{
					CC3D_PROFILE_SCOPE("LayerStack OnUpdate");

					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(timestep);
				}

				m_ImGuiLayer->Begin();
				{
					CC3D_PROFILE_SCOPE("LayerStack OnImGuiRender");

					for (Layer* layer : m_LayerStack)
						layer->OnImGuiRender();
				}
				m_ImGuiLayer->End();
			}

			m_Window->OnUpdate();
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		CC3D_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		CC3D_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		CC3D_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(CC3D_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(CC3D_BIND_EVENT_FN(Application::OnWindowResize));

		//CC3D_CORE_INFO("{0}", e);
		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) {
			if (e.Handled)
				break;
			(*it)->OnEvent(e);
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		CC3D_PROFILE_FUNCTION();

		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		CC3D_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

}