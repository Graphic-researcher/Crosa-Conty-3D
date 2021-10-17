# Lec24 Creating am Orthographic Camera

课程 : [here](https://www.youtube.com/watch?v=NjKv-HWstxA&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=35)

代码版本 : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/d01cf3d3b7c85d586cd6caff34007aaa8cee067a/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D)

先修课 : [Camera and How they work](https://www.youtube.com/watch?v=LfbqtmqxX04&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=34)

learnopenglZH : [Camera System](https://learnopengl-cn.github.io/01%20Getting%20started/09%20Camera/)

## Orthographic Camera 作用域

```c++
class OrthographicCamera
{
public:
	OrthographicCamera(float left, float right, float bottom, float top);

	const glm::vec3& GetPosition() const { return m_Position; }
	void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

	float GetRotation() const { return m_Rotation; }
	void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

	const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
	const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
	const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
private:
	void RecalculateViewMatrix();
private:
	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_ViewMatrix;
	glm::mat4 m_ViewProjectionMatrix;

	glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
	float m_Rotation = 0.0f;
};
OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
	: m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
{
	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}
void OrthographicCamera::RecalculateViewMatrix()
{
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
		glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

	m_ViewMatrix = glm::inverse(transform);
	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}
```

## Shader 作用域

```c++
class Shader
{
public:
	Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

private:
	uint32_t m_RendererID;
};
void Shader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}
```

## Render 作用域

```c++
class Renderer
{
public:
	static void BeginScene(OrthographicCamera& camera);
	static void EndScene();
	/// Submit material			   and		 		  geometry information
	static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);



	inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
private:
	struct SceneData
	{
		glm::mat4 ViewProjectionMatrix;
	};

	static SceneData* m_SceneData;


};

Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

void Renderer::BeginScene(OrthographicCamera& camera)
{
	m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
}
void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray)
{
	shader->Bind();
	shader->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);

	vertexArray->Bind();
	RenderCommand::DrawIndexed(vertexArray);
}
```

## Application 作用域

```c++
//Application.h:
OrthographicCamera m_Camera;
//Application.cpp:
OrthographicCamera test1(-1.6f, 1.6f, -0.9f, 0.9f);
OrthographicCamera test2(-1.0f, 1.0f, -1.0f, 1.0f);

Application::Application() : m_Camera(test1)
{
	//...code...

	m_VertexArray.reset(VertexArray::Create());

	//   COORDINATES        /     COLORS		 /
	float vertices[3 * 7] = {
		-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
		 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
	};

	std::shared_ptr<VertexBuffer> vertexBuffer;
	vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));


	BufferLayout layout = {
		{ ShaderDataType::Float3, "a_Position" },
		{ ShaderDataType::Float4, "a_Color" }
	};

	vertexBuffer->SetLayout(layout);
	m_VertexArray->AddVertexBuffer(vertexBuffer);


	uint32_t indices[3] = { 0, 1, 2 };
	std::shared_ptr<IndexBuffer> indexBuffer;
	indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
	m_VertexArray->SetIndexBuffer(indexBuffer);

	m_SquareVA.reset(VertexArray::Create());

	float squareVertices[3 * 4] = {
		-0.75f, -0.75f, 0.0f,///0
		 0.75f, -0.75f, 0.0f,///1
		 0.75f,  0.75f, 0.0f,///2
		-0.75f,  0.75f, 0.0f ///3
	};

	std::shared_ptr<VertexBuffer> squareVB;
	squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
	squareVB->SetLayout({
		{ ShaderDataType::Float3, "a_Position" }
		});
	m_SquareVA->AddVertexBuffer(squareVB);
	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	std::shared_ptr<IndexBuffer> squareIB;
	squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
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
				v_Color = a_Color;
				v_Position = a_Position;
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
				//color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

	m_Shader.reset(new Shader(vertexSrc, fragmentSrc));


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

	m_BlueShader.reset(new Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
}

void Application::Run()
{
	while (m_Running)
	{
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		m_Camera.SetPosition({ 0.5f, 0.5f, 0.0f });
		m_Camera.SetRotation(45.0f);

		Renderer::BeginScene(m_Camera);
		Renderer::Submit(m_BlueShader, m_SquareVA);
		Renderer::Submit(m_Shader, m_VertexArray);
		Renderer::EndScene();
		//...code...
	}
}
```

## 构建和结果

```c++
OrthographicCamera test2(-1.0f, 1.0f, -1.0f, 1.0f);
```

![image-20211017123145643](https://i.loli.net/2021/10/17/h2gwv531FjYMxVA.png)

```c++
OrthographicCamera test2(-1.0f, 1.0f, -1.0f, 1.0f);
///...code...
m_Camera.SetPosition({ 0.5f, 0.5f, 0.0f });
m_Camera.SetRotation(45.0f);
```

![image-20211017124042992](https://i.loli.net/2021/10/17/g42paKM9twRT3LC.png)