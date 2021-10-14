# Lec12 Imgui Events

课程 : [here](https://www.youtube.com/watch?v=yBP1gSbQPPM&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=16)

代码版本 : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/fb4c17160b8943c00ef16043751516cea98641c7/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D)

## Core 作用域

定义宏，之后用于 Imgui Layer 作用域

```c++
///see more details about this:
///std::bind(&XXX,this,std::placeholders::_1)
///https://stackoverflow.com/questions/61649318/how-could-c-stdbind-return-value-assigned-to-stdfunction
#define CC3D_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
```

## Event 作用域

添加 **KeyType** 枚举变量,之后用于 KeyEvent 作用域 和 Imgui Layer 作用域

```c++
enum class EventType
{
    None = 0,
    WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,//1 2 3 4 5
    AppTick, AppUpdate, AppRender,									     //6 7 8
    KeyPressed, KeyReleased, KeyTyped,										     //9 10
    MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled	 //11 12 13 14
};
```

## KeyEvent 作用域

```c++
class CC3D_API KeyTypedEvent : public KeyEvent
{
    public:
    KeyTypedEvent(int keycode)
    : KeyEvent(keycode) {}

    std::string ToString() const override
    {
    std::stringstream ss;
    ss << "KeyTypedEvent: " << m_KeyCode;
    return ss.str();
    }

    EVENT_CLASS_TYPE(KeyTyped)
};
```

## Imgui Layer 作用域

为了让我们的事件系统可以和imgui事件交互，添加 event dispatcher 

```c++
void ImGuiLayer::OnEvent(Event& event)
{
    EventDispatcher dispatcher(event);
    ///dispatcher.Dispatch<MouseButtonPressedEvent>(std::bind(&ImGuiLayer::OnMouseButtonPressedEvent,this,std::placeholders::_1));
    ///same as above
    dispatcher.Dispatch<MouseButtonPressedEvent>(CC3D_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
    dispatcher.Dispatch<MouseButtonReleasedEvent>(CC3D_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
    dispatcher.Dispatch<MouseMovedEvent>(CC3D_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
    dispatcher.Dispatch<MouseScrolledEvent>(CC3D_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
    dispatcher.Dispatch<KeyPressedEvent>(CC3D_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
    dispatcher.Dispatch<KeyTypedEvent>(CC3D_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
    dispatcher.Dispatch<KeyReleasedEvent>(CC3D_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
    dispatcher.Dispatch<WindowResizeEvent>(CC3D_BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));

}

bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
{
    ImGuiIO& io = ImGui::GetIO();
    io.MouseDown[e.GetMouseButton()] = true;
    return false;
}

bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
{
    ImGuiIO& io = ImGui::GetIO();
    io.MouseDown[e.GetMouseButton()] = false;
    return false;
}

bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
{
    ImGuiIO& io = ImGui::GetIO();
    io.MousePos = ImVec2(e.GetX(), e.GetY());
    return false;
}

bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
{
    ImGuiIO& io = ImGui::GetIO();
    io.MouseWheelH += e.GetXOffset();
    io.MouseWheel += e.GetYOffset();
    return false;
}

bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
{
    ImGuiIO& io = ImGui::GetIO();
    io.KeysDown[e.GetKeyCode()] = true;

    io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
    io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
    io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
    io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];///Super is Windows Key : Cmd/Super/Windows

    return false;
}

bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
{
    ImGuiIO& io = ImGui::GetIO();
    io.KeysDown[e.GetKeyCode()] = false;

    return false;
}

bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e)
{
    /// <summary>
    /// Using to type in textbox
    /// </summary>

    ImGuiIO& io = ImGui::GetIO();
    int keycode = e.GetKeyCode();
    if (keycode > 0 && keycode < 0x10000)
    io.AddInputCharacter((unsigned short)keycode);

    return false;
}

bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& e)
{
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
    io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
    glViewport(0, 0, e.GetWidth(), e.GetHeight());

    return false;
}
```

## WindowsWindow 作用域

添加glfwSetCharCallback，这样我们可以进行输入的回调

```c++
glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
{
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    KeyTypedEvent event(keycode);
    data.EventCallback(event);
});
```

## 构建和结果

你可以尝试点击imgui的按钮和工具栏，如果可以交互证明成功

![image-20211014134530006](https://i.loli.net/2021/10/14/pynkKCIhHGMsPNw.png)

![image-20211014134629373](https://i.loli.net/2021/10/14/xyXuaOrbLMGHWdV.png)

![image-20211014134751513](https://i.loli.net/2021/10/14/us8Wc7AraGT9nOM.png)

## 总结

![image-20211014170700579](https://i.loli.net/2021/10/14/CjSEXKNURLa82xT.png)