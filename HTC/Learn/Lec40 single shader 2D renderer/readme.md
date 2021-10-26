# Lec40 single shader 2D renderer

tutorial : [here](https://www.youtube.com/watch?v=-myXuS3t1W4&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=54)

code version : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/d0ed5fcaac8fe61612df7cb080aad29eac079edd/HTC/Project)

## Overview

The frequent switch of shaders could be a performance loss.

We can reduce our two shaders to one which means we don't need a shader to draw color and the other to draw texture, we can use texture shader do both

We generate a white texture through procedure and use that texture multiply the color and we can reach to the same function as before.

## Renderer Scope

### Render2D

```c++
struct Renderer2DStorage
{
    Ref<VertexArray> QuadVertexArray;
    Ref<Shader> TextureShader;
    Ref<Texture2D> WhiteTexture;
};

s_Data->WhiteTexture = Texture2D::Create(1, 1);
uint32_t whiteTextureData = 0xffffffff;//white color
///uint32_t whiteTextureData = 0x00000000;
s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
{
    s_Data->TextureShader->SetFloat4("u_Color", color);
    s_Data->WhiteTexture->Bind();
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { 	size.x, size.y, 1.0f });
    s_Data->TextureShader->SetMat4("u_Transform", transform);

    s_Data->QuadVertexArray->Bind();
    RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
}
```

### Texture

```c++
Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
{
    switch (Renderer::GetAPI())
    {
        case RendererAPI::API::None:    CC3D_CORE_ASSERT(false, "RendererAPI::None is currently not 		supported!"); return nullptr;
        case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTexture2D>(width, height);
    }

    CC3D_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}
```

### OpenGLTexture

```c++
OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
: m_Width(width), m_Height(height)
{
    m_InternalFormat = GL_RGBA8;
    m_DataFormat = GL_RGBA;

    glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
    glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

    glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
}
void OpenGLTexture2D::SetData(void* data, uint32_t size)
{
    uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
    CC3D_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
    glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, 		data);
}
```

### OpenGLRendererAPI

```c++
void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
{
    glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 			nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);
}
```

## Blend 

The render sequence is important because the blend process need source color which is the color to be put into pixel and  destination color which is the color in the frame buffer ,so you should render the opacity object, then render the semi transparent object for alpha blend.

Render opacity first [correct]

```c++
CC3D::Renderer2D::BeginScene(m_CameraController.GetCamera());
CC3D::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 1.0f, 1.0f }, m_CheckerboardTexture);//opacity 
CC3D::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });//transparent
CC3D::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 0.3f });
CC3D::Renderer2D::EndScene();
```

![image-20211026230140721](https://i.loli.net/2021/10/26/h8Zw2ESdsmp7lbM.png)

Render semi transparent first [wrong]

Because the transpart object will blend with black background

```
CC3D::Renderer2D::BeginScene(m_CameraController.GetCamera());
CC3D::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
CC3D::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 0.3f });
CC3D::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 1.0f, 1.0f }, m_CheckerboardTexture);
CC3D::Renderer2D::EndScene();
```

![image-20211026230241799](https://i.loli.net/2021/10/26/w3WztDkgBK9a1mf.png)

