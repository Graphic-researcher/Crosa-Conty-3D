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
///render
#include "CC3D/Renderer/Shader.h"
#include "CC3D/Renderer/Buffer.h"

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

		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }

	private:
		bool m_Running = true;
		///Window
		bool OnWindowClose(WindowCloseEvent& e);
		std::unique_ptr<Window> m_Window;
		///layer
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;
		///Render	
		unsigned int m_VertexArray;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		///App instance
		static Application* s_Instance;
	};
	//To be defined in CLIENT
	Application* CreateApplication();
	

}


