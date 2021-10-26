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
	m_SquareVA = CC3D::VertexArray::Create();

	float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};

	/// <summary>
	/// Create a Square VertexBuffer
	/// </summary>
	CC3D::Ref<CC3D::VertexBuffer> squareVB;
	squareVB = CC3D::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
	squareVB->SetLayout({
		{ CC3D::ShaderDataType::Float3, "a_Position" },
		{ CC3D::ShaderDataType::Float2, "a_TexCoord" }
		});
	m_SquareVA->AddVertexBuffer(squareVB);


	/// <summary>
	/// Create IndexBuffer for Square
	/// </summary>
	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	CC3D::Ref<CC3D::IndexBuffer> squareIB;
	squareIB = CC3D::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
	m_SquareVA->SetIndexBuffer(squareIB);


	auto TextureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");


	tex1 = CC3D::Texture2D::Create("assets/textures/waifu.png");//读取纹理时先要设置纹理
	tex2 = CC3D::Texture2D::Create("assets/textures/72137544_p0.png");

	std::dynamic_pointer_cast<CC3D::OpenGLShader>(TextureShader)->Bind();
	std::dynamic_pointer_cast<CC3D::OpenGLShader>(TextureShader)->UploadUniformInt("u_Texture", 0);
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

	CC3D::Renderer::BeginScene(m_CameraController.GetCamera());

	m_Texture = TexCheck ? tex1 : tex2;
	m_Texture->Bind();
	auto textureShader = m_ShaderLibrary.Get("Texture");

	CC3D::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	//CC3D::Renderer::Submit(m_Shader, m_VertexArray);

	CC3D::Renderer::EndScene();
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
