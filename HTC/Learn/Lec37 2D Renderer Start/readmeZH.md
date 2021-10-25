# Lec37 2D Renderer Start

课程: [here](https://www.youtube.com/watch?v=r_5fF1AxgpU&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=51)

版本管理 : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/9080a6c169d3023f5d2ec0a4915d6903f34ee1fc/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D)

## Render 作用域

### Renderer

```c++
void Renderer::Init()
{
    RenderCommand::Init();
    Renderer2D::Init();
}
```

### Renderer2D

```c++
class Renderer2D
{
public:
	static void Init();
	static void Shutdown();

	static void BeginScene(const OrthographicCamera& camera);
	static void EndScene();

	// Primitives
	static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
	static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
};
struct Renderer2DStorage
{
	Ref<VertexArray> QuadVertexArray;
	Ref<Shader> FlatColorShader;
};

static Renderer2DStorage* s_Data;

void Renderer2D::Init()
{
	s_Data = new Renderer2DStorage();
	s_Data->QuadVertexArray = VertexArray::Create();

	float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	Ref<VertexBuffer> squareVB;
	squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
	squareVB->SetLayout({
		{ ShaderDataType::Float3, "a_Position" }
		});
	s_Data->QuadVertexArray->AddVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	Ref<IndexBuffer> squareIB;
	squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
	s_Data->QuadVertexArray->SetIndexBuffer(squareIB);

	s_Data->FlatColorShader = Shader::Create("assets/shaders/FlatColor.glsl");
}

void Renderer2D::Shutdown()
{
	delete s_Data;
}

void Renderer2D::BeginScene(const OrthographicCamera& camera)
{
	std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->Bind();
	std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->UploadUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->UploadUniformMat4("u_Transform", glm::mat4(1.0f));
}

void Renderer2D::EndScene()
{

}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
	DrawQuad({ position.x, position.y, 0.0f }, size, color);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
{
	std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->Bind();
	std::dynamic_pointer_cast<CC3D::OpenGLShader>(s_Data->FlatColorShader)->UploadUniformFloat4("u_Color", color);

	s_Data->QuadVertexArray->Bind();
	RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
}
```

## Sandbox2D

```c++
void Sandbox2D::OnAttach()
{
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

	CC3D::Renderer2D::BeginScene(m_CameraController.GetCamera());
	CC3D::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	CC3D::Renderer2D::EndScene();


	// TODO: Add these functions - Shader::SetMat4, Shader::SetFloat4
	// std::dynamic_pointer_cast<CC3D::OpenGLShader>(m_FlatColorShader)->Bind();
	// std::dynamic_pointer_cast<CC3D::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);
}
```

## 构建和结果

现在还不可以更改颜色（材质），但可以操作相机，像之前那样

![image-20211025225347175](https://i.loli.net/2021/10/25/7ABYQVPJk9j5Six.png)