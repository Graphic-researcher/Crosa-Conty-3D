# Lec38 2D Renderer Transform

课程 : [here](https://www.youtube.com/watch?v=-Qt12lcAF0Y&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=52)

代码版本 : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/9d348733a8c9f7968a3b152b4079c0214d1a1852/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D)

## Render 作用域

### Shader

```c++
virtual void SetInt(const std::string& name, int value) = 0;
virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;
virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;
```

### OpenGLShader

```c++
void OpenGLShader::SetInt(const std::string& name, int value)
{
	UploadUniformInt(name, value);
}

void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
{
	UploadUniformFloat3(name, value);
}

void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
{
	UploadUniformFloat4(name, value);
}

void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
{
	UploadUniformMat4(name, value);
}
```

### Render2D

```c++
void Renderer2D::BeginScene(const OrthographicCamera& camera)
{
    s_Data->FlatColorShader->Bind();
    s_Data->FlatColorShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
}
void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
{
    s_Data->FlatColorShader->Bind();
    s_Data->FlatColorShader->SetFloat4("u_Color", color);

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { 	size.x, size.y, 1.0f });
    s_Data->FlatColorShader->SetMat4("u_Transform", transform);

    s_Data->QuadVertexArray->Bind();
    RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
}
```

## Sandbox2D 作用域

```c++
void Sandbox2D::OnUpdate(CC3D::Timestep ts)
{
	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	CC3D::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	CC3D::RenderCommand::Clear();

	CC3D::Renderer2D::BeginScene(m_CameraController.GetCamera());///set view matrix
    ///set model matrix
	CC3D::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	CC3D::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });			CC3D::Renderer2D::EndScene();
}
```

## 构建和结果

![image-20211026150602904](https://i.loli.net/2021/10/26/oWFuADgVCrws5E3.png)