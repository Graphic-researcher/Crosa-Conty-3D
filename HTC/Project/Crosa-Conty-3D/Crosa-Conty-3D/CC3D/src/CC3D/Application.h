#pragma once
#include "Core.h"//contain marcos
#include "CC3D/Events/Event.h"
#include "CC3D/Events/ApplicationEvent.h"
#include "Window.h"
namespace CC3D {
	//class _declspec(dllexport) Application
	class CC3D_API Application //use CC3D_API as replacement of _declspec(dllexport) 
	{
	public:
		Application();
		virtual ~Application();//this class would be inherited by other class
		void Run();

		void OnEvent(Event& e);
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};
	//To be defined in CLIENT
	Application* CreateApplication();
	

}


