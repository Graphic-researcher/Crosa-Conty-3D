# Lec13 Input Polling

tutorial : [here](https://youtube.com/watch?v=yuhNj8yGDJQ&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=19)

code version : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/6dd91819003eed5871e4481eff2c6e87f31e630f/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D)

## Overview

![image-20211014171936495](https://i.loli.net/2021/10/14/fFgVCkKrWNno9jc.png)

## Input Scope

```c++
class CC3D_API Input
{
    public:
    	inline static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode);}

        inline static bool IsMouseButtonPressed(int button) { return s_Instance-    >IsMouseButtonPressedImpl(button); }
        inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
        inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
        inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }
        protected:
        virtual bool IsKeyPressedImpl(int keycode) = 0;

        virtual bool IsMouseButtonPressedImpl(int button) = 0;
        virtual std::pair<float, float> GetMousePositionImpl() = 0;
        virtual float GetMouseXImpl() = 0;
        virtual float GetMouseYImpl() = 0;
    private:
    	static Input* s_Instance;
};

```

## WindowsInput Scope

```c++
class WindowsInput : public Input
{
    protected:
    virtual bool IsKeyPressedImpl(int keycode) override;
    virtual bool IsMouseButtonPressedImpl(int button) override;
    virtual std::pair<float, float> GetMousePositionImpl() override;
    virtual float GetMouseXImpl() override;
    virtual float GetMouseYImpl() override;
};

Input* Input::s_Instance = new WindowsInput();

bool WindowsInput::IsKeyPressedImpl(int keycode)
{
	auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
	auto state = glfwGetKey(window, keycode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool WindowsInput::IsMouseButtonPressedImpl(int button)
{
	auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
	auto state = glfwGetMouseButton(window, button);
	return state == GLFW_PRESS;
}

std::pair<float, float> WindowsInput::GetMousePositionImpl()
{
	auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	return { (float)xpos, (float)ypos };
}

float WindowsInput::GetMouseXImpl()
{
	/// C++17
	auto[x, y] = GetMousePositionImpl();
	return x;
}

float WindowsInput::GetMouseYImpl()
{
	auto[x, y] = GetMousePositionImpl();
	return y;
}
```

## Application Scope

If you hit A,the back-end will log true

If you click one place,the back-end will log the mouse position

See the result in "Build and Result" session

```c++
void Application::Run()
{
	gladLoadGL();//Load GLAD so it configures OpenGL
	while (m_Running)
	{
		glClearColor(1, 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		for (Layer* layer : m_LayerStack)
			layer->OnUpdate();
		auto[x, y] = Input::GetMousePosition();//new
		CC3D_CORE_TRACE("{0},{1}", x, y);
		auto x1 = Input::IsKeyPressed(GLFW_KEY_A);//new
		CC3D_CORE_TRACE("{0}", x1);
		m_Window->OnUpdate();
	}
}
```

## Build and Result

![image-20211014173253067](https://i.loli.net/2021/10/14/tCY3Wye71Huvdw8.png)