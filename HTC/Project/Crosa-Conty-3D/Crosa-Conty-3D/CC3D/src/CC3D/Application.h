#pragma once
#include "Core.h"//contain marcos
///event
#include "CC3D/Events/Event.h"
#include "CC3D/Events/ApplicationEvent.h"
///Window
#include "Window.h"
///layer
#include "CC3D/LayerStack.h"
#include "CC3D/ImGui/ImGuiLayer.h"
///Core
#include "CC3D/Core/Timestep.h"


namespace CC3D {
	class CC3D_API Application //use CC3D_API as replacement of _declspec(dllexport) 
	{
	public:
		Application();
		virtual ~Application() = default;//this class would be inherited by other class
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
		///Core
		float m_LastFrameTime = 0.0f;///for time step
		///Window
		bool OnWindowClose(WindowCloseEvent& e);
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


