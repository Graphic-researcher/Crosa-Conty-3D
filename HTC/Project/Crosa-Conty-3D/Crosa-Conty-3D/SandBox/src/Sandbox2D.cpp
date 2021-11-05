#include "Sandbox2D.h"
#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>

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

///use timer0 to avoid redefination with Instrumentor Profiling Scope
#define PROFILE_SCOPE(name) Timer timer0##__LINE__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })


Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
	CC3D_PROFILE_FUNCTION();

	m_CheckerboardTexture = CC3D::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
	CC3D_PROFILE_FUNCTION();
}


void Sandbox2D::TestDraw()
{
	//CC3D::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 1.0f, 1.0f }, m_CheckerboardTexture);
	//CC3D::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, m_CheckerboardTexture);
	//CC3D::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });///set model matrix
	CC3D::Renderer2D::DrawRotatedQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, glm::radians(45.0), { 0.8f, 0.2f, 0.3f, 1.0f });///set model matrix
	CC3D::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 0.3f });
	//CC3D::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
	//CC3D::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture);
	CC3D::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 1.0f, 1.0f }, m_CheckerboardTexture);
	//CC3D::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 10.0f, 10.0f }, m_CheckerboardTexture);
	//CC3D::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, m_CheckerboardTexture);
}

void Sandbox2D::TestDraw44()
{
	CC3D::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	CC3D::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
}

void Sandbox2D::TestDraw45()
{
	CC3D::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	CC3D::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
	CC3D::Renderer2D::DrawQuad({ -5.0f, -5.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture, 10.0f);
	CC3D::Renderer2D::DrawQuad({ -0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f }, m_CheckerboardTexture, 20.0f);
}

void Sandbox2D::TestDraw46(CC3D::Timestep ts)
{
	static float rotation = 0.0f;
	rotation += ts * 50.0f;
	CC3D::Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, -45.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
	CC3D::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	CC3D::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
	CC3D::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture, 10.0f);
	CC3D::Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, m_CheckerboardTexture, 20.0f);

}

void Sandbox2D::OnUpdate(CC3D::Timestep ts)
{
	CC3D_PROFILE_FUNCTION();

	PROFILE_SCOPE("Sandbox2D::OnUpdate");
	// Update
	m_CameraController.OnUpdate(ts);

	{
		CC3D_PROFILE_SCOPE("CameraController::OnUpdate");

		PROFILE_SCOPE("CameraController::OnUpdate");
		m_CameraController.OnUpdate(ts);
	}

	// Render
	{
		CC3D_PROFILE_SCOPE("Renderer Prep");

		PROFILE_SCOPE("Renderer Prep");
		CC3D::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		CC3D::RenderCommand::Clear();
	}

	{
		CC3D_PROFILE_SCOPE("Renderer Draw");

		PROFILE_SCOPE("Renderer Draw");
		CC3D::Renderer2D::BeginScene(m_CameraController.GetCamera());///set view matrix
		TestDraw46(ts);
		CC3D::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	CC3D_PROFILE_FUNCTION();

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

void Sandbox2D::OnEvent(CC3D::Event& e)
{
	m_CameraController.OnEvent(e);
}