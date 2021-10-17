# Lec25 Move to SandBox

教程 : [here](https://www.youtube.com/watch?v=4zj-0FQ7Xbg&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=36)

代码版本 : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/033acc1aebeb281925e087332b72ad6a5cf99e5f/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D)

现在我们希望引擎的核心部分更整洁，所以我们要将渲染相关代码放到客户端（sandbox）

## SandBox 作用域

```c++
//#include "../../CC3D/src/CC3D.h"
#include "CC3D.h"
#include "imgui/imgui.h"



class ExampleLayer : public CC3D::Layer
{
public:
	ExampleLayer()
		: Layer("Example"),m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
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
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
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

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
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

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
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


	void OnUpdate() override
	{
		///translation
		if (CC3D::Input::IsKeyPressed(CC3D_KEY_LEFT))
		{
			m_CameraPosition.x -= m_CameraMoveSpeed;
		}
		else if (CC3D::Input::IsKeyPressed(CC3D_KEY_RIGHT))
		{
			m_CameraPosition.x += m_CameraMoveSpeed;
		}
		if (CC3D::Input::IsKeyPressed(CC3D_KEY_DOWN))
		{
			m_CameraPosition.y -= m_CameraMoveSpeed;
		}
		else if (CC3D::Input::IsKeyPressed(CC3D_KEY_UP))
		{
			m_CameraPosition.y += m_CameraMoveSpeed;
		}


		///roation
		if (CC3D::Input::IsKeyPressed(CC3D_KEY_A))
			m_CameraRotation += m_CameraRotationSpeed;
		if (CC3D::Input::IsKeyPressed(CC3D_KEY_D))
			m_CameraRotation -= m_CameraRotationSpeed;

		CC3D::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		CC3D::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		CC3D::Renderer::BeginScene(m_Camera);

		CC3D::Renderer::Submit(m_BlueShader, m_SquareVA);
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
	float m_CameraMoveSpeed = 0.1f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 0.1f;

};
```

## Application 作用域

Application 作用域 变回添加渲染指令之前的状态

可以读这篇记录： [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/HTC/HTC/Learn/Lec17%20Render%20Context)

## 构建和结果

按 ↑↓←→ 可以移动摄像头,按 A/a 或 D/d 可以旋转摄像头

![CamTrans](./result/CamTrans.gif)