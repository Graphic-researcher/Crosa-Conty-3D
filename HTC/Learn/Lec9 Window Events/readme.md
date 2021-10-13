# Lec9 Window Events

tutorial : [here](https://www.youtube.com/watch?v=r74WxFMIEdU)

code version : [engine_project](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/0e328735ad10d4fe76aa447a7cfe9d29543e35f7/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D) [prerequisite_code](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/f2d902eeeab90acca55f1ff88a7f0450eefa803e/HTC/Project/CC3D)

prerequisite : [Function Pointers in C++](https://www.youtube.com/watch?v=p4sDgQ-jao4) 	[Lambdas in C++](https://www.youtube.com/watch?v=mWgmBBz0y8c) 

If you want to check the details about the changes,please use any kind of git tool to check the comparison of commits

![image-20211013125209896](https://i.loli.net/2021/10/13/XvtSmKpMcDaoEdZ.png)

## Implement glfw call back:

```c++
//set GLFW callbacks
///learn lambda:https://www.youtube.com/watch?v=mWgmBBz0y8c
///learn function pointer:https://www.youtube.com/watch?v=p4sDgQ-jao4
glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
{
	WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
	data.Width = width;
	data.Height = height;

	WindowResizeEvent event(width, height);
	data.EventCallback(event);
});
glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
{
	WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
	WindowCloseEvent event;
	data.EventCallback(event);
});
glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
	WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
	//Convert glfw keycode to our Engine's form
	switch (action)
	{
	case GLFW_PRESS:
	{
		KeyPressedEvent event(key, 0);
		data.EventCallback(event);
		break;
	}
	case GLFW_RELEASE:
	{
		KeyReleasedEvent event(key);
		data.EventCallback(event);
		break;
	}
	case GLFW_REPEAT:
	{
		KeyPressedEvent event(key, 1);
		data.EventCallback(event);
		break;
	}
	}
});
glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
	WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
	switch (action)
	{
	case GLFW_PRESS:
	{
		MouseButtonPressedEvent event(button);
		data.EventCallback(event);
		break;
	}
	case GLFW_RELEASE:
	{
		MouseButtonReleasedEvent event(button);
		data.EventCallback(event);
		break;
	}
	}
});

glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
{
	WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

	MouseScrolledEvent event((float)xOffset, (float)yOffset);
	data.EventCallback(event);
});

glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
{
	WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

	MouseMovedEvent event((float)xPos, (float)yPos);
	data.EventCallback(event);
});
```

```c++
static void GLFWErrorCallback(int error, const char* description)
{
	CC3D_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
}
```

## Test the code in Application Scope :

```c++
#define BIND_EVENT_FN(x) std::bind(&Application::x,this,std::placeholders::_1)
Application::Application()
{
	m_Window = std::unique_ptr<Window>(Window::Create());
	//m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
	m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
}
Application::~Application()
{
}
void Application::Run()
{
	while (m_Running)
	{
		glClearColor(1, 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		m_Window->OnUpdate();
	}
}
void Application::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
	CC3D_CORE_INFO("{0}", e);
}

bool Application::OnWindowClose(WindowCloseEvent& e)
{
	m_Running = false;
	return true;
}
```

![Lec9](./result/Lec9.gif)