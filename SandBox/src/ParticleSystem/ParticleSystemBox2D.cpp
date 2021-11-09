#include "ParticleSystemBox2D.h"

#include "Platform//OpenGL/OpenGLShader.h"

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

#include <chrono>

ParticleSystemBox2D::ParticleSystemBox2D()
	:Layer("SandBox2D"), m_CameraController(1280.0f / 720.0f, true)
{
}

void ParticleSystemBox2D::OnAttach()
{
	CC3D_PROFILE_FUNCTION();

	// Init here

	m_Particle.ColorBegin = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_Particle.ColorEnd = glm::vec4{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	//m_Particle.ColorBegin = glm::vec4{ 1.0f, 0.4f, 1.0f, 1.0f };
	//m_Particle.ColorEnd = glm::vec4{ 0.0f, 0.4f, 0.0f, 1.0f };
	m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
	m_Particle.LifeTime = 10.0f;
	m_Particle.Velocity = glm::vec2{ 0.0f, 0.0f };
	m_Particle.VelocityVariation = glm::vec2{ 3.0f, 1.0f };
	m_Particle.Position = glm::vec2{ 0.0f, 0.0f };
}

void ParticleSystemBox2D::OnDetach()
{
	CC3D_PROFILE_FUNCTION();
}

void ParticleSystemBox2D::OnUpdate(CC3D::Timestep ts)
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

		if (CC3D::Input::IsMouseButtonPressed(CC3D::MouseCode::ButtonLeft))
		{
			auto [x, y] = CC3D::Input::GetMousePosition();
			auto width = CC3D::Application::Get().GetWindow().GetWidth();
			auto height = CC3D::Application::Get().GetWindow().GetHeight();

			auto bounds = m_CameraController.GetBounds();
			auto pos = m_CameraController.GetCamera().GetPosition();
			x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
			y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
			m_Particle.Position = glm::vec2{ x + pos.x, y + pos.y };
			for (int i = 0; i < 5; i++)
				m_ParticleSystem.Emit(m_Particle);
		}

		m_ParticleSystem.OnUpdate(ts);
		m_ParticleSystem.OnRender(m_CameraController.GetCamera());

	}
}

void ParticleSystemBox2D::OnImGuiRender()
{
	CC3D_PROFILE_FUNCTION();

	ImGui::ShowDemoWindow();

	// Note: Switch this to true to enable dockspace
	static bool dockingEnabled = false;
	if (dockingEnabled)
	{
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;// 防止父窗口可以docking
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);//控制圆角
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);//控制边界宽度
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("Exit")) CC3D::Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::Begin("Settings");

		auto stats = CC3D::Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::End();
	}
	else
	{
		ImGui::Begin("Settings");

		auto stats = CC3D::Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::End();
	}
}

void ParticleSystemBox2D::OnEvent(CC3D::Event& e)
{
	m_CameraController.OnEvent(e);
}
