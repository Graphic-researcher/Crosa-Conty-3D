#include "cc3d_pch.h"
#include "Application.h"
#include "CC3D/Log.h"

#include "Input.h"




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

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	void Application::Run()
	{
		while (m_Running)
		{
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();
			
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
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