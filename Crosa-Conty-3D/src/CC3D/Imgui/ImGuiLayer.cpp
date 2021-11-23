#include "ccpch.h"
#include "CC3D/Imgui/ImGuiLayer.h"

#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include "CC3D/Core/Application.h"


// TEMPORARY
#include <GLFW/glfw3.h>
//#include <glad/glad.h>

#include "ImGuizmo.h"

namespace CC3D
{
	ImGuiLayer::ImGuiLayer()
		:Layer("ImGuiLayer")
	{

	}

	void ImGuiLayer::OnAttach()
	{
		CC3D_PROFILE_FUNCTION();

		//IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void) io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		//io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		//io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		float fontSize = 18.0f;// *2.0f;
		io.Fonts->AddFontFromFileTTF("Assets/fonts/arialbd.ttf", fontSize);
		io.FontDefault = io.Fonts->AddFontFromFileTTF("Assets/fonts/arial.ttf", fontSize);

		ImGui::StyleColorsLight();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;

		}

		SetWarmThemeColors();

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 460"); // GLAD version
		
	}
	void ImGuiLayer::OnDetach()
	{
		CC3D_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

	}

	void ImGuiLayer::OnEvent(Event& e)
	{
		if (m_BlockEvents)
		{
			ImGuiIO& io = ImGui::GetIO();
			e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}

	void ImGuiLayer::Begin()
	{
		CC3D_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}
	void ImGuiLayer::End()
	{ 
		CC3D_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}
	// TODO
	void ImGuiLayer::SetDarkThemeColors()
	{
	}
	void ImGuiLayer::SetWarmThemeColors()
	{
		// Border
		auto& WindowBorderSize = ImGui::GetStyle().WindowBorderSize;
		auto& ChildBorderSize = ImGui::GetStyle().ChildBorderSize;
		auto& PopupBorderSize = ImGui::GetStyle().PopupBorderSize;
		auto& FrameBorderSize = ImGui::GetStyle().FrameBorderSize;
		auto& TabBorderSize = ImGui::GetStyle().TabBorderSize;

		WindowBorderSize = 1;
		ChildBorderSize = 0;
		PopupBorderSize = 0;
		FrameBorderSize = 0;
		TabBorderSize = 0;

		// Rounding
		auto& FrameRounding = ImGui::GetStyle().FrameRounding;
		FrameRounding = 6;

		auto& colors = ImGui::GetStyle().Colors;

		// PopupBg
		colors[ImGuiCol_PopupBg] = ImVec4{ 0.745f, 0.776f, 0.949f, 1.0f };

		// Frame BG
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.745f, 0.776f, 0.949f, 1.0f };

		// Headers
		colors[ImGuiCol_Header] = ImVec4{ 0.894f, 0.482f, 0.643f, 0.31f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.894f, 0.482f, 0.643f, 0.463f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.894f, 0.482f, 0.643f, 0.588f };

		// Buttons
		colors[ImGuiCol_Button] = ImVec4{ 0.114f, 0.212f, 0.478f, 0.4f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.114f, 0.212f, 0.478f, 0.475f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.114f, 0.212f, 0.478f, 0.675f };

		//// Tabs
		//colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		//colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
		//colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
		//colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		//colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

		//// Title
		//colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		//colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		//colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	}

}



