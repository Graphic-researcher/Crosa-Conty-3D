# Lec26 TimeStep and DeltaTime

tutorial : [here](https://www.youtube.com/watch?v=pctGOMDW-HQ&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=37)

code version : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/d0671dd633dba275e35b40dd2677f6a646626555/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D)

We want to render in a rate we can control instead of fully depending on CPU cycles,so we need to define our own time step method.

## TimeStep Scope

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

## Layer Scope

```c++
//Layer.h
///Core
float m_LastFrameTime = 0.0f;///for time step
```

## Application Scope

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

## SandBox Scope

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



## Build and Result

```c++
m_Window->SetVSync(false);
```

![image-20211017170734612](https://i.loli.net/2021/10/17/rEyAH1dLP9sG8KJ.png)

```c++
m_Window->SetVSync(true);
```

![image-20211017170535098](https://i.loli.net/2021/10/17/BuWXlP3a4HLvngU.png)

If you apply time step to your sandbox's update() and transform the camera in program,the result should be similar.