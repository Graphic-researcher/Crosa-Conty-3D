# Lec40 single shader 2D renderer

课程 : [here](https://www.youtube.com/watch?v=-myXuS3t1W4&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=54)

代码版本 : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/d0ed5fcaac8fe61612df7cb080aad29eac079edd/HTC/Project)

## 总览

频繁切换shader会损耗性能，我们可以将两个shader（一个负责绘制颜色，一个负责绘制纹理）减成一个，即让绘制纹理的shader也可以绘制颜色。我们程序化生成一个白色纹理，让它传进texture shader，再乘以我们给予的颜色就可以达到之前的效果。

## Renderer 作用域

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

## 混合

渲染顺序很重要，因为alpha混合需要源颜色（即将绘制的颜色）和已经在帧缓冲的颜色做混合，所以你要先绘制不透明物体，再绘制透明物体。

先绘制不透明物体 [正确]

```c++
CC3D::Renderer2D::BeginScene(m_CameraController.GetCamera());
CC3D::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 1.0f, 1.0f }, m_CheckerboardTexture);//opacity 
CC3D::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });//transparent
CC3D::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 0.3f });
CC3D::Renderer2D::EndScene();
```

![image-20211026230140721](https://i.loli.net/2021/10/26/h8Zw2ESdsmp7lbM.png)

先绘制半透明物体 [错误]

因为透明物体会和背景混合

```
CC3D::Renderer2D::BeginScene(m_CameraController.GetCamera());
CC3D::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
CC3D::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 0.3f });
CC3D::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 1.0f, 1.0f }, m_CheckerboardTexture);
CC3D::Renderer2D::EndScene();
```

![image-20211026230241799](https://i.loli.net/2021/10/26/w3WztDkgBK9a1mf.png)

## OpenGL生成纹理

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

```c++
//Render2D.cpp
s_Data->WhiteTexture = Texture2D::Create(1, 1);
	//R G B A?	(wrong!) it's A B G R
uint32_t whiteTextureData = 0xffffffff;//white color
s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(whiteTextureData));
//Sandbox2D.cpp
CC3D::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
```

![image-20211027120124414](https://i.loli.net/2021/10/27/pbrmFgP4YsX5q2T.png)

```c++
//Render2D.cpp
s_Data->WhiteTexture = Texture2D::Create(1, 1);
	//R G B A?	(wrong!) it's A B G R
uint32_t whiteTextureData = 0xff00ffff;//yellow color
s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(whiteTextureData));
//Sandbox2D.cpp
CC3D::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
```

![image-20211027120519952](https://i.loli.net/2021/10/27/rYGmCeuzAJI7hRP.png)

```c++
//Render2D.cpp
s_Data->WhiteTexture = Texture2D::Create(1, 1);
					//R G B A?	(wrong!) it's A B G R
uint32_t whiteTextureData = 0xff0000ff;//yellow color
s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(whiteTextureData));
//Sandbox2D.cpp
CC3D::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
```

![image-20211027120648511](https://i.loli.net/2021/10/27/OVA2H4WJFQPIX7Y.png)

但这出现了一些问题:

```c++
//Render2D.cpp
s_Data->WhiteTexture = Texture2D::Create(1, 1);
		//R G B A?	(wrong!) it's A B G R
uint32_t whiteTextureData = 0xfff000ff;//yellow color
s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(whiteTextureData));
//Sandbox2D.cpp
CC3D::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
```

![image-20211027122109177](https://i.loli.net/2021/10/27/NrI5PW6oexXTaC4.png)

![image-20211027122203790](https://i.loli.net/2021/10/27/26Womwv4kuEij1p.png)

所以**glTextureSubImage2D**对图片数据的读取方式可能和我理解的有问题

最终我们发现

颜色数据读取的方式不是RGBA而是ABGR

```c++
s_Data->WhiteTexture = Texture2D::Create(2, 2);
//ABGR
uint32_t whiteTextureData[4] = {
    0xffffffff,//white
    0xff0000ff,//red
    0xff00ff00,//blue
    0xffff0000,//green
};
s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(whiteTextureData));
```

![image-20211027123814030](https://i.loli.net/2021/10/27/anWOT8FNrpjgdIk.png)

