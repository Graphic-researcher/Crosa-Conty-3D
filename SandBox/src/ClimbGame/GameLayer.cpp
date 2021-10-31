#include "GameLayer.h"

GameLayer::GameLayer()
	:Layer("ClimbGame")
{
	auto& window = CC3D::Application::Get().GetWindow();
	CreateCamera(window.GetWidth(), window.GetHeight());

	Random::Init();
}

void GameLayer::OnAttach()
{
	m_Level.Init();
}

void GameLayer::OnDetach()
{
}

void GameLayer::OnUpdate(CC3D::Timestep ts)
{
	m_Time += ts;
	
	// TODO GameOver
	if (m_Level.IsGameOver())
		m_State = GameState::GameOver;

	// ¸üÐÂÉãÏñÍ·Î»ÖÃ£¬ÉãÏñÍ·ÉÏÉý
	{
		cameraSpeed += std::log(m_Time + 1) * 0.001;
		auto pos = m_Camera->GetPosition();
		m_Camera->SetPosition(pos + glm::vec3(0.0f, cameraSpeed * ts, 0.0f));
	}
	// TODO ÅÐ¶ÏÊÇ·ñ³¬³öÉãÏñÍ··¶Î§
	{
		auto pos = m_Level.GetPlayer().GetPosition();
	}
	

	//if (CC3D::Input::IsKeyPressed(CC3D::KeyCode::W))
	//{
	//	auto pos = m_Camera->GetPosition();
	//	m_Camera->SetPosition(pos + glm::vec3(0.0f, cameraSpeed * ts, 0.0f));

	//}
	//if (CC3D::Input::IsKeyPressed(CC3D::KeyCode::A))
	//{
	//	auto pos = m_Camera->GetPosition();
	//	m_Camera->SetPosition(pos + glm::vec3(-cameraSpeed * ts, 0.0f, 0.0f));

	//}
	//if (CC3D::Input::IsKeyPressed(CC3D::KeyCode::S))
	//{
	//	auto pos = m_Camera->GetPosition();
	//	m_Camera->SetPosition(pos + glm::vec3(0.0f, -cameraSpeed * ts, 0.0f));

	//}
	//if (CC3D::Input::IsKeyPressed(CC3D::KeyCode::D))
	//{
	//	auto pos = m_Camera->GetPosition();
	//	m_Camera->SetPosition(pos + glm::vec3(cameraSpeed * ts, 0.0f, 0.0f));

	//}

	switch (m_State)
	{
		case GameState::Play:
		{
			m_Level.OnUpdate(ts);
			break;
		}
	}

	CC3D::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.3f, 1.0f));
	CC3D::RenderCommand::Clear();

	CC3D::Renderer2D::BeginScene(*m_Camera);
	m_Level.OnRender();
	CC3D::Renderer2D::EndScene();
}

void GameLayer::OnImGuiRender()
{
	ImGui::Begin("demo");
	ImGui::ShowDemoWindow();
	ImGui::End();
}

void GameLayer::OnEvent(CC3D::Event& e)
{
	CC3D::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<CC3D::WindowResizeEvent>(CC3D_BIND_EVENT_FN(GameLayer::OnWindowResize));
	dispatcher.Dispatch<CC3D::MouseButtonPressedEvent>(CC3D_BIND_EVENT_FN(GameLayer::OnMouseButtonPressed));
}

bool GameLayer::OnMouseButtonPressed(CC3D::MouseButtonPressedEvent& e)
{
	return false;
}

bool GameLayer::OnWindowResize(CC3D::WindowResizeEvent& e)
{
	CreateCamera(e.GetWidth(), e.GetHeight());
	return false;
}

void GameLayer::CreateCamera(uint32_t width, uint32_t height)
{
	float aspectRatio = (float)width / (float)height;

	float camWidth = 8.0f;
	float bottom = -camWidth;
	float top = camWidth;
	float left = bottom * aspectRatio;
	float right = top * aspectRatio;
	m_Camera = CC3D::CreateScope<CC3D::OrthographicCamera>(left, right, bottom, top);
}
