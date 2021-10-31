#pragma once
#include "CC3D/Core/Core.h"
///event
#include "CC3D/Events/Event.h"
#include "CC3D/Events/ApplicationEvent.h"
///Window
#include "CC3D/Core/Window.h"
///layer
#include "CC3D/Core/LayerStack.h"
#include "CC3D/ImGui/ImGuiLayer.h"
///Core
#include "CC3D/Core/Timestep.h"


namespace CC3D {
	class Application //use  as replacement of _declspec(dllexport) 
	{
	public:
		Application();
		virtual ~Application();
		void Run();
		///Events
		void OnEvent(Event& e);
		///layers
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }

	private:
		bool m_Running = true;
		bool m_Minimized = false;

		///Core
		float m_LastFrameTime = 0.0f;///for time step
		///Window
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		std::unique_ptr<Window> m_Window;
		///layer
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;
		///App instance
		static Application* s_Instance;
	};
	//To be defined in CLIENT
	Application* CreateApplication();
	

}


