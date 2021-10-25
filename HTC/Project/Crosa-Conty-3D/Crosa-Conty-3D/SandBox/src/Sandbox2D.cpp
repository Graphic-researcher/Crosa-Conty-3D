#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
	m_SquareVA = CC3D::VertexArray::Create();

	float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	CC3D::Ref<CC3D::VertexBuffer> squareVB;
	squareVB.reset(CC3D::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
	squareVB->SetLayout({
		{ CC3D::ShaderDataType::Float3, "a_Position" }
	});
	m_SquareVA->AddVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	CC3D::Ref<CC3D::IndexBuffer> squareIB;
	squareIB.reset(CC3D::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
	m_SquareVA->SetIndexBuffer(squareIB);

	m_FlatColorShader = CC3D::Shader::Create("assets/shaders/FlatColor.glsl");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(CC3D::Timestep ts)
{
	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	CC3D::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	CC3D::RenderCommand::Clear();

	CC3D::Renderer::BeginScene(m_CameraController.GetCamera());

	std::dynamic_pointer_cast<CC3D::OpenGLShader>(m_FlatColorShader)->Bind();
	std::dynamic_pointer_cast<CC3D::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

	CC3D::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	CC3D::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(CC3D::Event& e)
{
	m_CameraController.OnEvent(e);
}