#pragma once

#include "CC3D/Core/Base.h"
#include "CC3D/Events/ApplicationEvent.h"

#include "CC3D/Core/Window.h"
#include "CC3D/Core/LayerStack.h"
#include "CC3D/Events/Event.h"
#include "CC3D/Events/ApplicationEvent.h"

#include "CC3D/Imgui/ImGuiLayer.h"

#include "CC3D/Renderer/Shader.h"
#include "CC3D/Renderer/Buffer.h"
#include "CC3D/Renderer/VertexArray.h"

#include "CC3D/Renderer/OrthographicCamera.h"

#include "GLFW/glfw3.h"

int main(int argc, char** argv);

namespace CC3D {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		void PopLayer(Layer* layer);
		void PopOverlay(Layer* layer);

		static Application& Get() {
			return *s_Instance;
		}
		Window& GetWindow() { return *m_Window; }

		void Close();
	private:
		void Run();
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;

		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	Application* CreateApplication();
}

