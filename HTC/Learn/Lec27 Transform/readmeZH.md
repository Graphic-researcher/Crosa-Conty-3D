# Lec27 Transform

课程 : [here](https://www.youtube.com/watch?v=cbB2fh0UxZ0&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=38)

代码版本 : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/4fe2ea621b49de066aa5bac8196bca7d36362a21/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D)

## Render 作用域

```c++
/// Submit material			   and		 		  geometry information							   transform matrix
void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
{
    shader->Bind();
    shader->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
    shader->UploadUniformMat4("u_Transform", transform);

    vertexArray->Bind();
    RenderCommand::DrawIndexed(vertexArray);
}

```

## SandBox 作用域

```c++
ExampleLayer()
	: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f), m_SquareTransform(0.0f)
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
	CC3D_TRACE("Delta time:{0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());

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

	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 20; x++)
		{
			glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
			transform *= glm::translate(glm::mat4(1.0f), m_SquareTransform);
			CC3D::Renderer::Submit(m_BlueShader, m_SquareVA, transform);
		}
	}
	CC3D::Renderer::Submit(m_Shader, m_VertexArray);

	CC3D::Renderer::EndScene();
}
```

## 构建和结果

按I上移方块，K下移方块

此时，希望你对观察变换和模型变换有了更深的理解，当我们按I，K进行模型变换时，方块移动的方向正是我们希望其移动的方向，然后而方向键移动相机的时候发现，此时画面中的物体是以我们预期相反的方向移动的，这就是观察变换和模型变换之间的联系，观察变换可以理解成模型变换的逆变换，（反正最终变换的结果都要作用于顶点），所以我们会有相机这个概念，我们理解成我们在移动相机，实际上是空间中的物体在移动（逆变换）

![Trans](./Trans.gif)