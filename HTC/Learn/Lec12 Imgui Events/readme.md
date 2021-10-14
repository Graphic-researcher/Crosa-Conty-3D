# Lec12 Imgui Events

tutorial : [here](https://www.youtube.com/watch?v=yBP1gSbQPPM&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=16)

code version : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/fb4c17160b8943c00ef16043751516cea98641c7/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D)

## Core Scope

define marcos,we will use it in Imgui Layer Scope

```c++
///see more details about this:
///std::bind(&XXX,this,std::placeholders::_1)
///https://stackoverflow.com/questions/61649318/how-could-c-stdbind-return-value-assigned-to-stdfunction
#define CC3D_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
```

## Event Scope

Add **KeyType** to enum,we will use it in KeyEvent Scope and Imgui Layer Scope

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

## KeyEvent Scope

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

## Imgui Layer Scope

Add event dispatcher so that our event system could interact with imgui events

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

## WindowsWindow Scope

Add this glfw callback so that we could input text

```c++
glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
{
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    KeyTypedEvent event(keycode);
    data.EventCallback(event);
});
```

## Build and Result

You can click those buttons and toolbars

![image-20211014134530006](https://i.loli.net/2021/10/14/pynkKCIhHGMsPNw.png)

![image-20211014134629373](https://i.loli.net/2021/10/14/xyXuaOrbLMGHWdV.png)

![image-20211014134751513](https://i.loli.net/2021/10/14/us8Wc7AraGT9nOM.png)

## Sum up

//TODO replace it with the note in ipad

![image-20211014170700579](https://i.loli.net/2021/10/14/CjSEXKNURLa82xT.png)