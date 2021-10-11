#pragma once

#include "Core.h"
#include "CC3D/Events/ApplicationEvent.h"

#include "Window.h"
#include "CC3D/LayerStack.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

namespace CC3D {

	class CC3D_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	Application* CreateApplication();
}

