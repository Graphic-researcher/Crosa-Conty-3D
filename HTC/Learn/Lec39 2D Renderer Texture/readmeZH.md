# Lec39 2D Renderer Texture

课程 : [here](https://www.youtube.com/watch?v=mW8eW3pLtmk&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=53)

代码版本 : [here]()

## Render 作用域

### Render2D

```c++
struct Renderer2DStorage
{
	Ref<VertexArray> QuadVertexArray;
	Ref<Shader> FlatColorShader;
	Ref<Shader> TextureShader;
};

static Renderer2DStorage* s_Data;

void Renderer2D::Init()
{
	s_Data = new Renderer2DStorage();
	s_Data->QuadVertexArray = VertexArray::Create();
	///position        ///texcoord or uv
	float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};

	Ref<VertexBuffer> squareVB;
	squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
	squareVB->SetLayout({
		{ ShaderDataType::Float3, "a_Position" },
		{ ShaderDataType::Float2, "a_TexCoord" }
		});
	s_Data->QuadVertexArray->AddVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	Ref<IndexBuffer> squareIB;
	squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
	s_Data->QuadVertexArray->SetIndexBuffer(squareIB);

	s_Data->FlatColorShader = Shader::Create("assets/shaders/FlatColor.glsl");
	s_Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
	s_Data->TextureShader->Bind();
	s_Data->TextureShader->SetInt("u_Texture", 0);
}
void Renderer2D::BeginScene(const OrthographicCamera& camera)
{
	s_Data->FlatColorShader->Bind();
	s_Data->FlatColorShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

	s_Data->TextureShader->Bind();
	s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
{
	DrawQuad({ position.x, position.y, 0.0f }, size, texture);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
{
	s_Data->TextureShader->Bind();

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
	s_Data->TextureShader->SetMat4("u_Transform", transform);

	texture->Bind();

	s_Data->QuadVertexArray->Bind();
	RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
}
```

### OpenGLTexture

```c++
///texture wrap mode
glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
```

### OpenGLRendererAPI

```c++
void OpenGLRendererAPI::Init()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
}
```

## Sandbox2D 作用域

```c++
void Sandbox2D::OnAttach()
{
	m_CheckerboardTexture = CC3D::Texture2D::Create("assets/textures/Checkerboard.png");
}
void Sandbox2D::OnUpdate(CC3D::Timestep ts)
{
	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	CC3D::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	CC3D::RenderCommand::Clear();

	CC3D::Renderer2D::BeginScene(m_CameraController.GetCamera());
	//CC3D::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, m_CheckerboardTexture);
	CC3D::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	CC3D::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
	CC3D::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
	//CC3D::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture);
	CC3D::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.c++1f }, { 1.0f, 1.0f }, m_CheckerboardTexture);
	//CC3D::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 10.0f, 10.0f }, m_CheckerboardTexture);
	//CC3D::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, m_CheckerboardTexture);
	CC3D::Renderer2D::EndScene();
}
```

## 深度判断

```c++
CC3D::Renderer2D::BeginScene(m_CameraController.GetCamera());
CC3D::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
CC3D::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });	
CC3D::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
CC3D::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, m_CheckerboardTexture);
CC3D::Renderer2D::EndScene();
```

![image-20211026162229959](https://i.loli.net/2021/10/26/7Z2gyfpxQXW9IhH.png)

```c++
CC3D::Renderer2D::BeginScene(m_CameraController.GetCamera());
CC3D::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, m_CheckerboardTexture);
CC3D::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
CC3D::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });	
CC3D::Renderer2D::EndScene();
```

![image-20211026162505289](https://i.loli.net/2021/10/26/TUElCSjJIq6oGBb.png)

use z coordinate to correct the depth:

```c++
CC3D::Renderer2D::BeginScene(m_CameraController.GetCamera());
CC3D::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
CC3D::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
CC3D::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
CC3D::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 1.0f, 1.0f }, m_CheckerboardTexture);
CC3D::Renderer2D::EndScene();
```

![image-20211026162651402](https://i.loli.net/2021/10/26/UtG8eqmRLQ4Z1uX.png)