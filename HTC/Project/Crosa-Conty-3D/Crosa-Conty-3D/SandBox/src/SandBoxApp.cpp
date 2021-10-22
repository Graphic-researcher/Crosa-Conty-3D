//#include "../../CC3D/src/CC3D.h"
#include "CC3D.h"
#include "imgui/imgui.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>




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

		CC3D::Ref<CC3D::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(CC3D::VertexBuffer::Create(vertices, sizeof(vertices)));
		CC3D::BufferLayout layout = {
			{ CC3D::ShaderDataType::Float3, "a_Position" },
			{ CC3D::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		CC3D::Ref<CC3D::IndexBuffer> indexBuffer;
		indexBuffer.reset(CC3D::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(CC3D::VertexArray::Create());
								//TEXCOORD
		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};


		CC3D::Ref<CC3D::VertexBuffer> squareVB;
		squareVB.reset(CC3D::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ CC3D::ShaderDataType::Float3, "a_Position" },
			{ CC3D::ShaderDataType::Float2, "a_TexCoord" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		CC3D::Ref<CC3D::IndexBuffer> squareIB;
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

		m_Shader = CC3D::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);

		std::string flatColorShaderVertexSrc = R"(
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
		m_ShaderLibrary.Add(m_FlatColorShader);///add shader to shaderlib
		/// Draw Plate UV
		std::string UVShaderVertexSrc = R"(
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

		std::string UVShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec2 v_TexCoord;


			void main()
			{
				color = vec4(v_TexCoord, 0.0, 1.0);
			}
		)";

		m_UVShader = CC3D::Shader::Create("UVColor",UVShaderVertexSrc, UVShaderFragmentSrc);
		m_ShaderLibrary.Add(m_UVShader);///add shader to shaderlib
		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		m_Texture = CC3D::Texture2D::Create("assets/textures/Checkerboard.png");
		m_ChernoLogoTexture = CC3D::Texture2D::Create("assets/textures/ChernoLogo.png");

		std::dynamic_pointer_cast<CC3D::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<CC3D::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);

	}


	void OnUpdate(CC3D::Timestep ts) override
	{
		///Log the delta time
		CC3D_TRACE("Delta time:{0}s ({1}ms)",ts.GetSeconds(), ts.GetMilliseconds());

		///Camera translation
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


		///Camera roation
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

		auto TestShader = m_ShaderLibrary.Get("UVColor");//find shader by name in shaderlibray
		
		std::dynamic_pointer_cast<CC3D::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<CC3D::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);



		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				transform*=glm::translate(glm::mat4(1.0f),m_SquareTransform);
				CC3D::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}

		///draw uv
		glm::vec3 pos( 1.5f,0.0f, 0.0f);
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)* glm::scale(glm::mat4(1.0f), glm::vec3(1.5f));
		///CC3D::Renderer::Submit(m_UVShader, m_SquareVA, transform);//use shader to sumbit
		CC3D::Renderer::Submit(TestShader, m_SquareVA,transform);//sumbit by shaderlib
		auto textureShader = m_ShaderLibrary.Get("Texture");
		///draw texture
		m_Texture->Bind();
		CC3D::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_ChernoLogoTexture->Bind();
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
	}

private:
	///Render	
	CC3D::Ref<CC3D::Shader> m_Shader;
	CC3D::Ref<CC3D::VertexArray> m_VertexArray;
	CC3D::Ref<CC3D::Shader> m_FlatColorShader,m_UVShader;
	CC3D::Ref<CC3D::VertexArray> m_SquareVA;
	CC3D::Ref<CC3D::Texture2D> m_Texture,m_ChernoLogoTexture;
	CC3D::OrthographicCamera m_Camera;
	CC3D::ShaderLibrary m_ShaderLibrary;

	///Render Data
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 5.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 180.0f;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };


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