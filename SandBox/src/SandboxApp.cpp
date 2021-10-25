#include <CC3D.h>

#include "imgui.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer: public CC3D::Layer
{
public:
	ExampleLayer()
		:Layer("Example"), m_CameraController(1280.0f / 720.0f, true)
	{
		m_VertexArray.reset(CC3D::VertexArray::Create());

		/// <summary>
		/// Create a new VertexBuffer
		/// </summary>
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		float vertices2[3 * 7] = {
			-0.1f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.7f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.9f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};


		CC3D::Ref<CC3D::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(CC3D::VertexBuffer::Create(vertices, sizeof(vertices)));

		CC3D::Ref<CC3D::VertexBuffer> vertexBuffer2;
		vertexBuffer2.reset(CC3D::VertexBuffer::Create(vertices2, sizeof(vertices2)));

		CC3D::BufferLayout layout = {
			{CC3D::ShaderDataType::Float3, "a_Position"},
			{CC3D::ShaderDataType::Float4, "a_Color"}
		};

		CC3D::BufferLayout layout2 = {
			{CC3D::ShaderDataType::Float3, "a_Position"},
			{CC3D::ShaderDataType::Float4, "a_Color"}
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		vertexBuffer2->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer2);


		/// <summary>
		/// Create a new IndexBuffer
		/// </summary>
		unsigned int indices[3] = { 0,1,2 };
		CC3D::Ref<CC3D::IndexBuffer> indexBuffer;
		indexBuffer.reset(CC3D::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(CC3D::VertexArray::Create());

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
		squareVB.reset(CC3D::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
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
		squareIB.reset(CC3D::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			layout(location = 2) in vec3 b_Position;
			layout(location = 3) in vec4 b_Color;

			uniform mat4 u_ViewProjection;	
			uniform mat4 u_Transform;
			
			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position + b_Position;
				v_Color = a_Color + b_Color/5;
				gl_Position =  u_ViewProjection * u_Transform * vec4(a_Position + b_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_Shader = CC3D::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);


		std::string flatColorShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			out vec3 v_Position;

			uniform mat4 u_ViewProjection;	
			uniform mat4 u_Transform;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";
		
		m_FlatColorShader = CC3D::Shader::Create("FlatColor", flatColorShaderVertexSrc, flatColorShaderFragmentSrc);

		std::dynamic_pointer_cast<CC3D::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<CC3D::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);
	

		
		auto TextureShader=m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		m_Texture = CC3D::Texture2D::Create("assets/textures/waifu.png");//读取纹理时先要设置纹理
		m_Tex = CC3D::Texture2D::Create("assets/textures/72137544_p0.png");

		std::dynamic_pointer_cast<CC3D::OpenGLShader>(TextureShader)->Bind();
		std::dynamic_pointer_cast<CC3D::OpenGLShader>(TextureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(CC3D::Timestep ts) override
	{
		// Camera
		m_CameraController.OnUpdate(ts);

		// Render
		CC3D::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1));
		CC3D::RenderCommand::Clear();

		CC3D::Renderer::BeginScene(m_CameraController.GetCamera());

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));



		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				if ((x + y) % 2 == 0)
				{
					m_SquareColor = glm::vec3(0.8f, 0.3f, 0.3f);
					std::dynamic_pointer_cast<CC3D::OpenGLShader>(m_FlatColorShader)->Bind();
					std::dynamic_pointer_cast<CC3D::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);
				}
				else
				{
					m_SquareColor = glm::vec3(0.2f, 0.3f, 0.8f);
					std::dynamic_pointer_cast<CC3D::OpenGLShader>(m_FlatColorShader)->Bind();
					std::dynamic_pointer_cast<CC3D::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);
				}
					
				CC3D::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}

		m_Texture->Bind();
		auto textureShader = m_ShaderLibrary.Get("Texture");

		CC3D::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		//CC3D::Renderer::Submit(m_Shader, m_VertexArray);

		CC3D::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(CC3D::Event& event) override
	{
		m_CameraController.OnEvent(event);
	}
private:
	CC3D::ShaderLibrary m_ShaderLibrary;
	CC3D::Ref<CC3D::Shader> m_Shader;
	CC3D::Ref<CC3D::VertexArray> m_VertexArray;

	CC3D::Ref<CC3D::Shader> m_FlatColorShader;
	CC3D::Ref<CC3D::VertexArray> m_SquareVA;

	CC3D::Ref<CC3D::Texture2D> m_Texture;
	CC3D::Ref<CC3D::Texture2D> m_Tex;

	CC3D::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquareColor = glm::vec3(0.2f, 0.3f, 0.8f);
};

class Sandbox : public CC3D::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};

CC3D::Application* CC3D::CreateApplication()
{
	return new Sandbox();
}