# Lec26 TimeStep and DeltaTime

课程 : [here](https://www.youtube.com/watch?v=pctGOMDW-HQ&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=37)

代码版本 : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/d0671dd633dba275e35b40dd2677f6a646626555/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D)

我们希望以可控稳定的频率渲染画面而不是完全依靠CPU的周期来决定每帧的时间，所以我们需要定义自己的时间步长

## TimeStep 作用域

```c++
class Timestep
{
public:
	Timestep(float time = 0.0f)
		: m_Time(time)
	{
	}

	operator float() const { return m_Time; }

	float GetSeconds() const { return m_Time; }
	float GetMilliseconds() const { return m_Time * 1000.0f; }
private:
	float m_Time;
};
```

## Layer 作用域

```c++
//Layer.h
///Core
float m_LastFrameTime = 0.0f;///for time step
```

## Application 作用域

```c++
////Application.h
virtual void OnUpdate(Timestep ts) {}///call when layer update
//Application.cpp
void Application::Run()
{
	while (m_Running)
	{
		///time step
		float time = (float)glfwGetTime(); ///Platform : GetTime()
		Timestep timestep = time - m_LastFrameTime;
		m_LastFrameTime = time;


		for (Layer* layer : m_LayerStack)
			layer->OnUpdate(timestep);

		m_ImGuiLayer->Begin();
		for (Layer* layer : m_LayerStack)
			layer->OnImGuiRender();
		m_ImGuiLayer->End();

		m_Window->OnUpdate();
	}
}
```

## SandBox 作用域

```c++
void OnUpdate(CC3D::Timestep ts) override
{
	///Log the delta time
	CC3D_TRACE("Delta time:{0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());

	///translation
	if (CC3D::Input::IsKeyPressed(CC3D_KEY_LEFT))
	{
		m_CameraPosition.x -= m_CameraMoveSpeed * ts;
	}
	else if (CC3D::Input::IsKeyPressed(CC3D_KEY_RIGHT))
	{
		m_CameraPosition.x += m_CameraMoveSpeed * ts;
	}
	if (CC3D::Input::IsKeyPressed(CC3D_KEY_DOWN))
	{
		m_CameraPosition.y -= m_CameraMoveSpeed * ts;
	}
	else if (CC3D::Input::IsKeyPressed(CC3D_KEY_UP))
	{
		m_CameraPosition.y += m_CameraMoveSpeed * ts;
	}

	//...transform the camera

	CC3D::Renderer::EndScene();
}
```



## 构建和结果

```c++
//in Application.cpp
m_Window->SetVSync(false);
```

![image-20211017170734612](https://i.loli.net/2021/10/17/rEyAH1dLP9sG8KJ.png)

```c++
//Application.cpp
m_Window->SetVSync(true);
```

![image-20211017170535098](https://i.loli.net/2021/10/17/BuWXlP3a4HLvngU.png)

If you apply time step to your sandbox's update() and transform the camera in program,the result should be similar.

如果你将时间步长应用于SandBox中的Update函数，当你变换相机时，结果[帧率，直观感受]应该相似