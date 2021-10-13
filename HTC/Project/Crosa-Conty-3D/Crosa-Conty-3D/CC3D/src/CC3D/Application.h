#pragma once
#include "Core.h"//contain marcos
///event
#include "CC3D/Events/Event.h"
#include "CC3D/Events/ApplicationEvent.h"
///Window
#include "Window.h"
///layer
#include "CC3D/LayerStack.h"



namespace CC3D {
	//class _declspec(dllexport) Application
	class CC3D_API Application //use CC3D_API as replacement of _declspec(dllexport) 
	{
	public:
		Application();
		virtual ~Application();//this class would be inherited by other class
		void Run();
		///Events
		void OnEvent(Event& e);
		///layers
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
	private:
		bool m_Running = true;
		///Window
		bool OnWindowClose(WindowCloseEvent& e);
		std::unique_ptr<Window> m_Window;
		///layer
		LayerStack m_LayerStack;
	};
	//To be defined in CLIENT
	Application* CreateApplication();
	

}


