#pragma once

#include "Core.h"
#include "CC3D/Events/ApplicationEvent.h"

#include "Window.h"
#include "CC3D/LayerStack.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

#include "CC3D/Imgui/ImGuiLayer.h"

#include "CC3D/Renderer/Shader.h"
#include "CC3D/Renderer/Buffer.h"
#include "CC3D/Renderer/VertexArray.h"

#include "Renderer/OrthographicCamera.h"

#include "GLFW/glfw3.h"

namespace CC3D {

	class Application
	{
	public:
		Application();
		virtual ~Application() = default;

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		void PopLayer(Layer* layer);
		void PopOverlay(Layer* layer);

		inline static Application& Get() {
			return *s_Instance;
		}

		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();
}

