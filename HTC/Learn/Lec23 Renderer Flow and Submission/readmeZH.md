# Lec23 Renderer Flow and Submission

课程 : [here](https://www.youtube.com/watch?v=akxevYYWd9g&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=33)

代码版本 : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/d922301f76d9c2442794920bde1040d2e3333b3a/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D)

## 总览

![image-20211017081549167](https://i.loli.net/2021/10/17/js1yprRdgPGWqtE.png)

## Render 作用域

### RenderAPI 作用域

```c++
class OpenGLRendererAPI : public RendererAPI
{
public:
	virtual void SetClearColor(const glm::vec4& color) override;
	virtual void Clear() override;

	virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
};
RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;
```

### RenderCommand 作用域

```c++
class RenderCommand
{
public:
	inline static void SetClearColor(const glm::vec4& color)
	{
		s_RendererAPI->SetClearColor(color);
	}

	inline static void Clear()
	{
		s_RendererAPI->Clear();
	}

	inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
	{
		s_RendererAPI->DrawIndexed(vertexArray);
	}
private:
	static RendererAPI* s_RendererAPI;
};
RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
```

### VertexArray 作用域

```c++
VertexArray* VertexArray::Create()
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::None:    CC3D_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
	case RendererAPI::API::OpenGL:  return new OpenGLVertexArray();
	}

	CC3D_CORE_ASSERT(false, "Unknown RendererAPI!");
	return nullptr;
}
```

### OpenGLRendererAPI 作用域

```c++
class OpenGLRendererAPI : public RendererAPI
{
public:
	virtual void SetClearColor(const glm::vec4& color) override;
	virtual void Clear() override;

	virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
};
void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRendererAPI::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
{
	glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
}
```

## Application 作用域

```c++
void Application::Run()
{
	while (m_Running)
	{
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		Renderer::BeginScene();

		m_BlueShader->Bind();
		m_SquareVA->Bind();
		Renderer::Submit(m_SquareVA);

		m_Shader->Bind();

		m_VertexArray->Bind();
		Renderer::Submit(m_VertexArray);

		Renderer::EndScene();

		//...code...
	}
}
```

