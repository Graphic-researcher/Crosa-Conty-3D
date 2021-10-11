# Lec8 Window Abstraction

## 类视图

![image-20211010230700329](https://i.loli.net/2021/10/10/3729VLBrNKtwpv1.png)

## 窗口文件

### Window Class:

作为接口会被实现其他平台的窗口类继承

```c++
#pragma once

#include "cc3d_pch.h"

#include "CC3D/Core.h"
#include "CC3D/Events/Event.h"

namespace CC3D {

	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "CC3D Engine",
			unsigned int width = 1280,
			unsigned int height = 720)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	// Interface representing a desktop system based Window
	class CC3D_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};

}
```

### WindowsWindow Class:

Windows平台

```c++
//.h file
#pragma once

#include "CC3D/Window.h"

#include <GLFW/glfw3.h>

namespace CC3D {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;//glfw  swap buffers

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;

		//pass WindowData to GLFW
		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

}
```

```c++
//.cpp file
#include "cc3d_pch.h"
#include "WindowsWindow.h"

namespace CC3D {
	
	static bool s_GLFWInitialized = false;

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		CC3D_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized)
		{
			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			CC3D_CORE_ASSERT(success, "Could not intialize GLFW!");

			s_GLFWInitialized = true;
		}

		///check Cherno's opengl series to learn more about glfw
		///https://www.youtube.com/watch?v=W3gAzLwfIP0&list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2
		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

}
```

Other file

在Core.h中实现如下宏指令（断点debug）

```c++
//for Window
///check certain conditons,log message
///if it fails,it would call debugbreak -> set a debug point on that line,to see what's wrong
#ifdef CC3D_ENABLE_ASSERTS
	#define CC3D_ASSERT(x, ...) { if(!(x)) { CC3D_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define CC3D_CORE_ASSERT(x, ...) { if(!(x)) { CC3D_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define CC3D_ASSERT(x, ...)
	#define CC3D_CORE_ASSERT(x, ...)
#endif
```

## Output

在Application类中写入窗口成员然后运行测试代码，出现大紫窗口说明成功了

```c++
//.h
class CC3D_API Application //use CC3D_API as replacement of _declspec(dllexport) 
{
    public:
    	Application();
    	virtual ~Application();//this class would be inherited by other class
    	void Run();
    private:
   	 	std::unique_ptr<Window> m_Window;
    	bool m_Running = true;
};
```

```c++
//.cpp
void Application::Run()
{
    while (m_Running)
    {
        glClearColor(1, 0, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        m_Window->OnUpdate();
    }
}
```

![image-20211010230648133](https://i.loli.net/2021/10/10/mFu4yZ2oRbD85ah.png)