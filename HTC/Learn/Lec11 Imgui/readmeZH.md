# Lec11 Imgui

tutorial : [here](https://www.youtube.com/watch?v=st4lgNI6_F4&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=15)

code version : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/6c6c7d2bdf47027f3ce1a9bafb304382829445e6/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D)

## ImGuiOpenGLRenderer 作用域

这部分文件其实是imgui库里2个文件改个名字

可以点击这里看 : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/6c6c7d2bdf47027f3ce1a9bafb304382829445e6/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D/CC3D/src/Platform/OpenGL)

## ImGuiLayer 作用域

![image-20211014171734845](https://i.loli.net/2021/10/14/SfZasBwqtnYcQJy.png)

```c++
class CC3D_API ImGuiLayer : public Layer
{
public:
	ImGuiLayer();
	~ImGuiLayer();

	void OnAttach();
	void OnDetach();
	void OnUpdate();
	void OnEvent(Event& event);
private:
	float m_Time = 0.0f;
};

ImGuiLayer::ImGuiLayer()
    : Layer("ImGuiLayer")
    {
    }

ImGuiLayer::~ImGuiLayer()
{
}

void ImGuiLayer::OnAttach()
{
    ImGui::CreateContext();
    ///ImGui::StyleColorsDark();
    ImGui::StyleColorsClassic();
    ///ImGui::StyleColorsLight();
    ImGuiIO& io = ImGui::GetIO();
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

    // TEMPORARY: should eventually use CC3D key codes
    io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
    io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
    io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
    io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
    io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
    io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
    io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
    io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
    io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
    io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
    io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
    io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
    io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
    io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
    io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
    io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
    io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

    ImGui_ImplOpenGL3_Init("#version 410");
}

void ImGuiLayer::OnDetach()
{

}

void ImGuiLayer::OnUpdate()
{
    ImGuiIO& io = ImGui::GetIO();
    Application& app = Application::Get();
    io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

    float time = (float)glfwGetTime();
    io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
    m_Time = time;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    static bool show = true;
    ImGui::ShowDemoWindow(&show);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiLayer::OnEvent(Event& event)
{

}
```

## Application 作用域

```c++
Application* Application::s_Instance = nullptr;
Application::Application()
{
    CC3D_CORE_ASSERT(!s_Instance, "Application already exists!");
    s_Instance = this;

    m_Window = std::unique_ptr<Window>(Window::Create());///same as below
    ///m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
    m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
}

void Application::PushLayer(Layer* layer)
{
    m_LayerStack.PushLayer(layer);
    layer->OnAttach();
}

void Application::PushOverlay(Layer* layer)
{
    m_LayerStack.PushOverlay(layer);
    layer->OnAttach();
}

void Application::Run()
{
    /// !!!!very very very important here!!!!
    /// If have the error of 0X00000 with glad 
    /// The solution is below:
    /// gladLoadGL();
    /// refer:
    /// https://stackoverflow.com/questions/67400482/access-violation-executing-location-0x0000000000000000-opengl-with-glad-and-glf
    gladLoadGL();//Load GLAD so it configures OpenGL

    while (m_Running)
    {
        glClearColor(1, 0, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        for (Layer* layer : m_LayerStack)
            layer->OnUpdate();
        m_Window->OnUpdate();
    }
	}
```



## SandBox 作用域

```c++
SandBox()
{
    PushLayer(new ExampleLayer());
    PushOverlay(new CC3D::ImGuiLayer());
}
```

## 你可能遇到的问题

![image-20211013225024022](https://i.loli.net/2021/10/13/SyltJis1oC2YUqu.png)

**解决方式:** 

在Application的Run函数加入下列代码，它将配置opengl

```c++
gladLoadGL();//Load GLAD so it configures OpenGL
```

reference ：

https://stackoverflow.com/questions/67400482/access-violation-executing-location-0x0000000000000000-opengl-with-glad-and-glf

## 构建和结果

![image-20211013224714026](https://i.loli.net/2021/10/13/x8qiKJDsd7aH53u.png)