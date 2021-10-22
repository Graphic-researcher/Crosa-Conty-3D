#include <CC3D.h>

#include "imgui.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer: public CC3D::Layer
{
public:
	ExampleLayer()
		:Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
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

		m_Shader.reset(CC3D::Shader::Create(vertexSrc, fragmentSrc));


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
		
		m_FlatColorShader.reset(CC3D::Shader::Create(flatColorShaderVertexSrc, flatColorShaderFragmentSrc));

		std::dynamic_pointer_cast<CC3D::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<CC3D::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);
	
		std::string textureShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec2 v_TexCoord;

			void main()
			{
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string textureShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec2 v_TexCoord;
			
			uniform sampler2D u_Texture;

			void main()
			{
				color = texture(u_Texture, v_TexCoord);
			}
		)";
		
		m_TextureShader.reset(CC3D::Shader::Create(textureShaderVertexSrc, textureShaderFragmentSrc));

		m_Texture = CC3D::Texture2D::Create("assets/textures/waifu.png");//读取纹理时先要设置纹理
		m_Tex = CC3D::Texture2D::Create("assets/textures/72137544_p0.png");

		std::dynamic_pointer_cast<CC3D::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<CC3D::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(CC3D::Timestep ts) override
	{
		if (CC3D::Input::IsKeyPressed(CC3D_KEY_TAB))
			CC3D_TRACE("Tab key is pressed(poll)!");

		if (CC3D::Input::IsKeyPressed(CC3D_KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		else if (CC3D::Input::IsKeyPressed(CC3D_KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;

		if (CC3D::Input::IsKeyPressed(CC3D_KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		else if (CC3D::Input::IsKeyPressed(CC3D_KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;

		if (CC3D::Input::IsKeyPressed(CC3D_KEY_A))
			m_CameraRotation += m_CameraRotationSpeed * ts;
		if (CC3D::Input::IsKeyPressed(CC3D_KEY_D))
			m_CameraRotation -= m_CameraRotationSpeed * ts;

		CC3D::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1));
		CC3D::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		CC3D::Renderer::BeginScene(m_Camera);

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
		CC3D::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_Tex->Bind();
		CC3D::Renderer::Submit(m_TextureShader, m_SquareVA);

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
		//if (event.GetEventType() == CC3D::EventType::KeyPressed)
		//{
		//	CC3D::KeyPressedEvent& e = (CC3D::KeyPressedEvent&)event;
		//	if (e.GetKeyCode() == CC3D_KEY_TAB)
		//		CC3D_TRACE("Tab key is pressed(event)!");
		//	CC3D_TRACE("{0}", (char)e.GetKeyCode());
		//}
	}
private:
	std::shared_ptr<CC3D::Shader> m_Shader;
	std::shared_ptr<CC3D::VertexArray> m_VertexArray;

	std::shared_ptr<CC3D::Shader> m_FlatColorShader, m_TextureShader;
	std::shared_ptr<CC3D::VertexArray> m_SquareVA;

	CC3D::Ref<CC3D::Texture2D> m_Texture;
	CC3D::Ref<CC3D::Texture2D> m_Tex;

	CC3D::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 5.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 180.0f;

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