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

	ImGuiIO io = ImGui::GetIO();
	m_Font = io.Fonts->AddFontFromFileTTF("src/ClimbGame/ClimbGame/Fonts/OpenSans-Regular.ttf", 120.0f);
}

void GameLayer::OnDetach()
{
}

void GameLayer::OnUpdate(CC3D::Timestep ts)
{
	
	
	// TODO GameOver
	if (m_Level.IsGameOver())
		m_State = GameState::GameOver;

	 
	// TODO 判断是否超出摄像头范围
	{
		auto pos = m_Level.GetPlayer().GetPosition();
		if (m_Camera->GetPosition().y - pos.y > 10)
		{
			m_State = GameState::GameOver;
		}
	}
	

	/*if (CC3D::Input::IsKeyPressed(CC3D::KeyCode::W))
	{
		auto pos = m_Camera->GetPosition();
		m_Camera->SetPosition(pos + glm::vec3(0.0f, cameraSpeed * ts, 0.0f));
		m_Level.ChangeCameraPosition(m_Camera->GetPosition());
	}
	if (CC3D::Input::IsKeyPressed(CC3D::KeyCode::A))
	{
		auto pos = m_Camera->GetPosition();
		m_Camera->SetPosition(pos + glm::vec3(-cameraSpeed * ts, 0.0f, 0.0f));
		m_Level.ChangeCameraPosition(m_Camera->GetPosition());
	}
	if (CC3D::Input::IsKeyPressed(CC3D::KeyCode::S))
	{
		auto pos = m_Camera->GetPosition();
		m_Camera->SetPosition(pos + glm::vec3(0.0f, -cameraSpeed * ts, 0.0f));
		m_Level.ChangeCameraPosition(m_Camera->GetPosition());
	}
	if (CC3D::Input::IsKeyPressed(CC3D::KeyCode::D))
	{
		auto pos = m_Camera->GetPosition();
		m_Camera->SetPosition(pos + glm::vec3(cameraSpeed * ts, 0.0f, 0.0f));
		m_Level.ChangeCameraPosition(m_Camera->GetPosition());
	}*/


	switch (m_State)
	{
		case GameState::Play:
		{
			// 更新摄像头位置，摄像头上升
			{
				cameraSpeed = std::log2f(m_Time + 1);
				auto pos = m_Camera->GetPosition();
				m_Camera->SetPosition(pos + glm::vec3(0.0f, cameraSpeed * ts, 0.0f));
				m_Level.ChangeCameraPosition(m_Camera->GetPosition());
			}
			m_Level.OnUpdate(ts);
			m_Time += ts;
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
	switch (m_State)
	{
		case GameState::Play:// Player
		{
			auto pos = ImGui::GetWindowPos();
			auto width = CC3D::Application::Get().GetWindow().GetWidth();
			auto height = CC3D::Application::Get().GetWindow().GetHeight();
			pos.x += width * 0.5f - 600.0f;
			pos.y += 600.0f;
			uint32_t playerScore = m_Level.GetPlayer().GetScore();
			std::string scoreStr = std::string("Score: ") + std::to_string(playerScore);
			ImGui::GetForegroundDrawList()->AddText(m_Font, 48.0f, pos, 0xffffffff, scoreStr.c_str());
			break;
		}
		case GameState::MainMenu:
		{
			auto pos = ImGui::GetWindowPos();
			auto width = CC3D::Application::Get().GetWindow().GetWidth();
			auto height = CC3D::Application::Get().GetWindow().GetHeight();
			pos.x += width * 0.5f - 300.0f;
			pos.y += 50.0f;
			ImGui::GetForegroundDrawList()->AddText(m_Font, 120.0f, pos, 0xffffffff, "Click to Play!");
			break;
		}
		case GameState::GameOver:
		{
			auto pos = ImGui::GetWindowPos();
			auto width = CC3D::Application::Get().GetWindow().GetWidth();
			auto height = CC3D::Application::Get().GetWindow().GetHeight();
			pos.x += width * 0.5f - 300.0f;
			pos.y += 50.0f;
			ImGui::GetForegroundDrawList()->AddText(m_Font, 120.0f, pos, 0xffffffff, "Click to Play!");

			pos.x += 200.0f;
			pos.y += 150.0f;
			uint32_t playerScore = m_Level.GetPlayer().GetScore();
			std::string scoreStr = std::string("Score: ") + std::to_string(playerScore);
			ImGui::GetForegroundDrawList()->AddText(m_Font, 48.0f, pos, 0xffffffff, scoreStr.c_str());
			break;
		}

	}

}

void GameLayer::OnEvent(CC3D::Event& e)
{
	CC3D::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<CC3D::WindowResizeEvent>(CC3D_BIND_EVENT_FN(GameLayer::OnWindowResize));
	dispatcher.Dispatch<CC3D::MouseButtonPressedEvent>(CC3D_BIND_EVENT_FN(GameLayer::OnMouseButtonPressed));
}

bool GameLayer::OnMouseButtonPressed(CC3D::MouseButtonPressedEvent& e)
{
	if (m_State == GameState::GameOver)
	{
		m_Level.Reset();
		m_Camera->SetPosition(glm::vec3(0.0, 0.0, 0.0));
		m_Time = 0;
	}

	m_State = GameState::Play;
	return false;
}

bool GameLayer::OnWindowResize(CC3D::WindowResizeEvent& e)
{
	// TODO  最小化窗口，相机位置会变为0
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
