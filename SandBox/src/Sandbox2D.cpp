#include "Sandbox2D.h"
#include <imgui.h>

#include "Platform//OpenGL/OpenGLShader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

Sandbox2D::Sandbox2D()
	:Layer("SandBox2D"), m_CameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(CC3D::Timestep ts)
{
	// Camera
	m_CameraController.OnUpdate(ts);

	// Render
	CC3D::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1));
	CC3D::RenderCommand::Clear();

	CC3D::Renderer2D::BeginScene(m_CameraController.GetCamera());
	CC3D::Renderer2D::DrawQuad(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec4(0.8f, 0.2f, 0.3f, 1.0f ));
	CC3D::Renderer2D::EndScene();

	// TODO: Add these functions - Shader::SetMat4, Shader::SetFloat4
	// std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->Bind();
	// std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Texture");

	ImGui::Checkbox("checkbox", &TexCheck);

	ImGui::End();
}

void Sandbox2D::OnEvent(CC3D::Event& e)
{
	m_CameraController.OnEvent(e);
}
