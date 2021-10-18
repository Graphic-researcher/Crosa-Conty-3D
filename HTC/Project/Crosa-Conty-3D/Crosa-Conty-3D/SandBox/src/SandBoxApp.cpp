//#include "../../CC3D/src/CC3D.h"
#include "CC3D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>



class ExampleLayer : public CC3D::Layer
{
public:
	ExampleLayer()
		: Layer("Example"),m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f),m_SquareTransform(0.0f)
	{
		m_VertexArray.reset(CC3D::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		std::shared_ptr<CC3D::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(CC3D::VertexBuffer::Create(vertices, sizeof(vertices)));
		CC3D::BufferLayout layout = {
			{ CC3D::ShaderDataType::Float3, "a_Position" },
			{ CC3D::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<CC3D::IndexBuffer> indexBuffer;
		indexBuffer.reset(CC3D::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(CC3D::VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		std::shared_ptr<CC3D::VertexBuffer> squareVB;
		squareVB.reset(CC3D::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ CC3D::ShaderDataType::Float3, "a_Position" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<CC3D::IndexBuffer> squareIB;
		squareIB.reset(CC3D::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
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

		m_Shader.reset(new CC3D::Shader(vertexSrc, fragmentSrc));

		std::string blueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string blueShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";

		m_BlueShader.reset(new CC3D::Shader(blueShaderVertexSrc, blueShaderFragmentSrc));

	}


	void OnUpdate(CC3D::Timestep ts) override
	{
		///Log the delta time
		CC3D_TRACE("Delta time:{0}s ({1}ms)",ts.GetSeconds(), ts.GetMilliseconds());

		///translation
		if (CC3D::Input::IsKeyPressed(CC3D_KEY_LEFT))
		{
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		}
		else if (CC3D::Input::IsKeyPressed(CC3D_KEY_RIGHT))
		{
			m_CameraPosition.x += m_CameraMoveSpeed * ts;
		}
		if (CC3D::Input::IsKeyPressed(CC3D_KEY_DOWN))
		{
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;
		}
		else if (CC3D::Input::IsKeyPressed(CC3D_KEY_UP))
		{
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		}


		///roation
		if (CC3D::Input::IsKeyPressed(CC3D_KEY_A))
			m_CameraRotation += m_CameraRotationSpeed * ts;
		if (CC3D::Input::IsKeyPressed(CC3D_KEY_D))
			m_CameraRotation -= m_CameraRotationSpeed * ts;

		
		///model transform
		if (CC3D::Input::IsKeyPressed(CC3D_KEY_I))
		{
			m_SquareTransform.y += m_SquareMoveSpeed * ts;
		}
		else if (CC3D::Input::IsKeyPressed(CC3D_KEY_K))
		{
			m_SquareTransform.y -= m_SquareMoveSpeed * ts;
		}
		
		
		
		CC3D::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
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
				transform*=glm::translate(glm::mat4(1.0f),m_SquareTransform);
				CC3D::Renderer::Submit(m_BlueShader, m_SquareVA, transform);
			}
		}
		CC3D::Renderer::Submit(m_Shader, m_VertexArray);

		CC3D::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
	}

	void OnEvent(CC3D::Event& event) override
	{
	}

private:
	///Render	
	std::shared_ptr<CC3D::Shader> m_Shader;
	std::shared_ptr<CC3D::VertexArray> m_VertexArray;
	std::shared_ptr<CC3D::Shader> m_BlueShader;
	std::shared_ptr<CC3D::VertexArray> m_SquareVA;
	CC3D::OrthographicCamera m_Camera;
	///Render Data
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 5.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 180.0f;

	///Transform Test
	glm::vec3 m_SquareTransform;
	float m_SquareMoveSpeed = 7.0f;
};

class SandBox : public CC3D::Application
{
public:
	SandBox()
	{
		PushLayer(new ExampleLayer());
	}
	~SandBox() {}

};

CC3D::Application* CC3D::CreateApplication()
{
	return new SandBox();
}