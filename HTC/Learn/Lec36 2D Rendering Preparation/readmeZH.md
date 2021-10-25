# Lec36 2D Rendering Preparation

课程 : [here](https://www.youtube.com/watch?v=sEiZZ2APlDs&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=50)

代码版本 : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/ccd045e1a847af4023d590168ec5961848d2870b/HTC)

## 总览

Hazel的仓库合并了其他人的pull request来修bug或者提高代码效率，我也抄了对应提交的部分，所以两者（hazel和CC3D）目前仍然是平行的，你可以查看版本历史来看更改的细节。

下面我们将开始2D渲染环节的准备。

## Vertex Array 作用域

```c++
Ref<VertexArray> VertexArray::Create()
{
    switch (Renderer::GetAPI())
    {
        case RendererAPI::API::None:    CC3D_CORE_ASSERT(false, "RendererAPI::None is currently not 		supported!"); return nullptr;
        case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLVertexArray>();
    }
    CC3D_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}
```

## Sand2D (new) 作用域

```c++
class Sandbox2D : public CC3D::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(CC3D::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(CC3D::Event& e) override;
private:
	CC3D::OrthographicCameraController m_CameraController;
	
	// Temp
	CC3D::Ref<CC3D::VertexArray> m_SquareVA;
	CC3D::Ref<CC3D::Shader> m_FlatColorShader;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};
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
```

## SandBoxApp 作用域

```c++
SandBox()
{
	PushLayer(new Sandbox2D());
}
```

## Build and Result

![image-20211025145921593](https://i.loli.net/2021/10/25/Tjbw1DnCq4YyvIu.png)