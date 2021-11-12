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

int main(int argc, char** argv);
namespace CC3D {
	class Application //use  as replacement of _declspec(dllexport) 
	{
	public:
		Application(const std::string& name = "CC3D App");
		virtual ~Application();
		///Events
		void OnEvent(Event& e);
		///layers
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		Window& GetWindow() { return *m_Window; }

		static Application& Get() { return *s_Instance; }

		void Close();
		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

	private:
		bool m_Running = true;
		bool m_Minimized = false;

		void Run();
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

		friend int ::main(int argc, char** argv);
	};
	//To be defined in CLIENT
	Application* CreateApplication();
	

}


