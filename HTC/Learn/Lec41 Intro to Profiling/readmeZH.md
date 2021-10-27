# Lec41 Intro to Profiling

课程 : [here](https://www.youtube.com/watch?v=YbYV8rRo9_A&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=55)

代码版本 : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/9ebc057b6b371a5b305346d2dc34750e84d5100a/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D)

先修课 : [Timing in C++](https://www.youtube.com/watch?v=oEx5vGNFrLk&list=PLlrATfBNZ98dudnM48yfGUldqGD0S4FFb&index=63)	[BENCHMARKING in C++ (how to measure performance)](https://www.youtube.com/watch?v=YG4jexlSAjc&list=PLlrATfBNZ98dudnM48yfGUldqGD0S4FFb&index=74)

## Sandbox2D 作用域

```c++
class Sandbox2D : public CC3D::Layer
{
public:
	//...
private:
	//...

	//profiling
	struct ProfileResult
	{
		const char* Name;
		float Time;
	};

	std::vector<ProfileResult> m_ProfileResults;

};
///check cherno's C++ series to learn about Timer:
///https://www.youtube.com/watch?v=oEx5vGNFrLk&list=PLlrATfBNZ98dudnM48yfGUldqGD0S4FFb&index=63
template<typename Fn>
class Timer
{
public:
	Timer(const char* name, Fn&& func)
		: m_Name(name), m_Func(func), m_Stopped(false)
	{
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		if (!m_Stopped)
			Stop();
	}

	void Stop()
	{
		auto endTimepoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		m_Stopped = true;

		float duration = (end - start) * 0.001f;
		m_Func({ m_Name, duration });
	}
private:
	const char* m_Name;
	Fn m_Func;
	std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
	bool m_Stopped;
};
#define PROFILE_作用域(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })

void Sandbox2D::OnUpdate(CC3D::Timestep ts)
{
	PROFILE_作用域("Sandbox2D::OnUpdate");
	// Update
	m_CameraController.OnUpdate(ts);

	{
		PROFILE_作用域("CameraController::OnUpdate");
		m_CameraController.OnUpdate(ts);
	}

	// Render
	{
		PROFILE_作用域("Renderer Prep");
		CC3D::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		CC3D::RenderCommand::Clear();
	}

	{
		PROFILE_作用域("Renderer Draw");
		CC3D::Renderer2D::BeginScene(m_CameraController.GetCamera());
		CC3D::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		CC3D::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 0.3f });
		CC3D::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 1.0f, 1.0f }, m_CheckerboardTexture);
		CC3D::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	
	///profiling information visualization
	for (auto& result : m_ProfileResults)
	{
		char label[50];
		strcpy(label, "%.3fms ");
		strcat(label, result.Name);
		ImGui::Text(label, result.Time);
	}
	m_ProfileResults.clear();
	
	
	ImGui::End();
}
```

## 构建和结果

![image-20211027153242235](https://i.loli.net/2021/10/27/W1h54sC7AoTSG9b.png)