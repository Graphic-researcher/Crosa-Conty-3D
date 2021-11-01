# Lec43 Improve 2D Renderer API

课程 : [here](https://www.youtube.com/watch?v=Q86V5DnSPPs&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=58)

代码版本 ：[here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/c8b046c5cd1102233e0d70cebc4d570d53c79dce/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D)

## Renderer 作用域

### Shader

```c++
virtual void SetFloat(const std::string& name, float value) = 0;
```

### OpenGL shader

```c++
void OpenGLShader::SetFloat(const std::string& name, float value)
{
    CC3D_PROFILE_FUNCTION();
    UploadUniformFloat(name, value);
}
```

### Render2D

```c++
void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
	DrawQuad({ position.x, position.y, 0.0f }, size, color);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
{
    CC3D_PROFILE_FUNCTION();

    s_Data->TextureShader->SetFloat4("u_Color", color);
    s_Data->TextureShader->SetFloat("u_TilingFactor", 1.0f);
    s_Data->WhiteTexture->Bind();
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
    * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
    s_Data->TextureShader->SetMat4("u_Transform", transform);
    s_Data->QuadVertexArray->Bind();
    RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
{
	DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
{
    CC3D_PROFILE_FUNCTION();

    s_Data->TextureShader->SetFloat4("u_Color", tintColor);
    s_Data->TextureShader->SetFloat("u_TilingFactor", tilingFactor);
    texture->Bind();

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
    * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
    s_Data->TextureShader->SetMat4("u_Transform", transform);

    s_Data->QuadVertexArray->Bind();
    RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
}

void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
{
	DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
}

void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
{
    CC3D_PROFILE_FUNCTION();

    s_Data->TextureShader->SetFloat4("u_Color", color);
    s_Data->TextureShader->SetFloat("u_TilingFactor", 1.0f);
    s_Data->WhiteTexture->Bind();

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
    * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
    * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
    s_Data->TextureShader->SetMat4("u_Transform", transform);
    s_Data->QuadVertexArray->Bind();
    RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
}

void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
{
	DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, 			tintColor);
}

void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
{
    CC3D_PROFILE_FUNCTION();

    s_Data->TextureShader->SetFloat4("u_Color", tintColor);
    s_Data->TextureShader->SetFloat("u_TilingFactor", tilingFactor);
    texture->Bind();

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
    * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
    * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
    s_Data->TextureShader->SetMat4("u_Transform", transform);

    s_Data->QuadVertexArray->Bind();
    RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
}
```

### Texture.glsl

```glsl
#type fragment
#version 330 core

layout(location = 0) out vec4 color;
in vec2 v_TexCoord;
uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform float u_TilingFactor;

void main()
{
	color = texture(u_Texture, v_TexCoord * u_TilingFactor) * u_Color;
}
```

## Sandbox2D 作用域

```c++
CC3D::Renderer2D::DrawRotatedQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, glm::radians(45.0),{ 0.8f, 0.2f, 0.3f, 1.0f });
CC3D::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 0.3f });
CC3D::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 1.0f, 1.0f }, m_CheckerboardTexture);
```

## 构建和结果

![image-20211101141937904](https://i.loli.net/2021/11/01/hSuTa2BPRl9AipW.png)