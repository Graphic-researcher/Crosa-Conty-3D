# Lec17 Render Context

教程 : [here](https://www.youtube.com/watch?v=YZKEjaCnsjU&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=27)

代码版本 : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/d0f232c8334be6a6a1a0d143a590b35b33033407/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D)

## 总览

If we want to support different platform and use different graphics api such as vulkan,d3d...

We need to have different render context method

So it's better to abstract render context and we can use different render context Object conveniently

如果我们想支持不同平台（利用不同图形api），我们需要不同的render context方法

所以我们需要对opengl的render context进行抽象

![image-20211015153639755](https://i.loli.net/2021/10/15/Av4DnrxZEu3UTql.png)

## Graphic context 作用域

```c++
class OpenGLContext : public GraphicsContext
{
public:
	OpenGLContext(GLFWwindow* windowHandle);

	virtual void Init() override;
	virtual void SwapBuffers() override;
private:
	GLFWwindow* m_WindowHandle;
};
OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
	: m_WindowHandle(windowHandle)
{
	CC3D_CORE_ASSERT(windowHandle, "Window handle is null!")
}
void OpenGLContext::Init()
{
	glfwMakeContextCurrent(m_WindowHandle);
	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	CC3D_CORE_ASSERT(status, "Failed to initialize Glad!");
}
void OpenGLContext::SwapBuffers()
{
	glfwSwapBuffers(m_WindowHandle);
}
```

## Windows 作用域

```c++
//add memeber in WindowsWindow.h
GraphicsContext* m_Context;

//modify method in WindowsWindow.cpp
void WindowsWindow::Init(const WindowProps& props)
{
//...code...
    m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
//using our render context method
    m_Context = new OpenGLContext(m_Window);
    m_Context->Init();

    glfwSetWindowUserPointer(m_Window, &m_Data);
    SetVSync(true);

//set GLFW callbacks
//...code...
	}
```

