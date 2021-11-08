#include "Sandbox2D.h"
#include <imgui.h>

#include "Platform//OpenGL/OpenGLShader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

#include <chrono>

Sandbox2D::Sandbox2D()
	:Layer("SandBox2D"), m_CameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
	CC3D_PROFILE_FUNCTION();

	m_WaifuTexture = CC3D::Texture2D::Create("assets/textures/waifualpha.png");
	m_SAGATexture = CC3D::Texture2D::Create("assets/textures/72137544_p0.png");
}

void Sandbox2D::OnDetach()
{
	CC3D_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(CC3D::Timestep ts)
{
	CC3D_PROFILE_FUNCTION();

	// Camera
	m_CameraController.OnUpdate(ts);


	// Render
	CC3D::Renderer2D::ResetStats();
	{
		CC3D_PROFILE_SCOPE("Renderer Prepare");
		CC3D::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		CC3D::RenderCommand::Clear();
	}

	{
		CC3D_PROFILE_SCOPE("Renderer Draw");
		static float rotation = 0.0f;
		rotation += ts * 50.0f;

		CC3D::Renderer2D::BeginScene(m_CameraController.GetCamera());
	
		
		CC3D::Renderer2D::DrawQuad(glm::vec2(-1.0f, 0.0f), glm::vec2(0.8f, 0.8f), glm::vec4(1.0f, 1.0f, 1.0f, 0.5f));
		CC3D::Renderer2D::DrawQuad(glm::vec2{ -1.0f, 0.0f }, glm::vec2{ 0.8f, 0.8f }, glm::vec4{ 0.8f, 0.2f, 0.3f, 1.0f });
		CC3D::Renderer2D::DrawQuad(glm::vec2{ 0.5f, -0.5f }, glm::vec2{ 0.5f, 0.75f }, glm::vec4{ 0.2f, 0.3f, 0.8f, 1.0f });
		//CC3D::Renderer2D::DrawQuad(glm::vec3(0.0f, 0.0f, -0.1f), glm::vec2(10.0f, 10.0f), m_WaifuTexture,10.f);
		CC3D::Renderer2D::DrawRotatedQuad(glm::vec3{ -2.0f, 0.0f, 0.0f }, glm::vec2{ 2.0f, 2.0f }, rotation, m_SAGATexture);
		CC3D::Renderer2D::EndScene();

		CC3D::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = glm::vec4{ (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
				CC3D::Renderer2D::DrawQuad(glm::vec2{ x, y }, glm::vec2{ 0.45f, 0.45f }, color);
			}
		}
		CC3D::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	CC3D_PROFILE_FUNCTION();

	ImGui::Begin("Settings");

	auto stats = CC3D::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::End();
}

void Sandbox2D::OnEvent(CC3D::Event& e)
{
	m_CameraController.OnEvent(e);
}
