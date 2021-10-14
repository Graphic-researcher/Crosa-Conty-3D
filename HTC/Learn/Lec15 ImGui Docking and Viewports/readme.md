# Lec15 ImGui Docking and Viewports

tutorial : [here](https://www.youtube.com/watch?v=lZuje-3iyVE&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=22)

code version :  [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/6b9f2427ad675a4dedd492a598fa5012b9ad4635/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D)

This version has a big change,we want to use more features (docking) of imgui so we refactor our code. Using git tool to check this history node to explore the comparison of code.

Be careful!This version has both deletion and implement of files.

## Refactoring of Imgui Scope

Get rid of **ImGuiOpenGLRender** scope , we will use other imgui files as our ui method.

File Implements : [vendor/imgui/examples/*](vendor/imgui/examples/*)

Include them into our code :

```c++
//ImGuiBuild.cpp
#include "cc3d_pch.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "imgui/examples/imgui_impl_opengl3.h"
#include "imgui/examples/imgui_impl_glfw.h"
```

```c++
class CC3D_API ImGuiLayer : public Layer
{
public:
	ImGuiLayer();
	~ImGuiLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnImGuiRender() override;


	void Begin();///begin window rendering
	void End();///end window rendering

private:
	float m_Time = 0.0f;
private:

};

void ImGuiLayer::OnAttach()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	Application& app = Application::Get();
	GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 410");
}

void ImGuiLayer::OnDetach()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiLayer::Begin()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiLayer::End()
{
	ImGuiIO& io = ImGui::GetIO();
	Application& app = Application::Get();
	io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

	// Rendering
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

void ImGuiLayer::OnImGuiRender()
{
	static bool show = true;
	ImGui::ShowDemoWindow(&show);
}
```

## Refactoring of Layer Scope

implement:

```c++
//layer.h
virtual void OnImGuiRender() {}
```

## Refactoring of LayerStack Scope

Modify m_LayerInsert to m_LayerInsertIndex

```c++
//LayerStack.h
unsigned int m_LayerInsertIndex = 0;
```

```c++

void LayerStack::PushLayer(Layer* layer)
{
    m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
    m_LayerInsertIndex++;
}
void LayerStack::PopLayer(Layer* layer)
{
    auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
    if (it != m_Layers.end())
    {
        m_Layers.erase(it);
        m_LayerInsertIndex--;
    }
}
```

## Build and Result

![image-20211014231328725](https://i.loli.net/2021/10/14/xqPYMceJGydNh8v.png)

![image-20211014231210289](https://i.loli.net/2021/10/14/X3NdwMUkZ2JAuS1.png)

![image-20211014231116629](https://i.loli.net/2021/10/14/AGlzbe1B3QJYhZ8.png)