# Lec 56 Entities and Components 

课程: [here](https://www.youtube.com/watch?v=-B1iu4QJTUc&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=78)

代码版本 : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/00edf8bde1508d196141233d16c9722d0daf432a/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D)

先修课 : [Entity Component System](https://www.youtube.com/watch?v=Z-CILn2w9K0&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=76) [Intro to ENTT](https://www.youtube.com/watch?v=D4hz0wEB978&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=78)

## 总览

![image-20211114133340910](/overview.png)

## Component 作用域

```c++
struct TransformComponent
{
    glm::mat4 Transform{ 1.0f };

    TransformComponent() = default;
    TransformComponent(const TransformComponent&) = default;
    TransformComponent(const glm::mat4 & transform)
        : Transform(transform) {}

    operator glm::mat4& () { return Transform; }
    operator const glm::mat4& () const { return Transform; }
};

struct SpriteRendererComponent
{
    glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

    SpriteRendererComponent() = default;
    SpriteRendererComponent(const SpriteRendererComponent&) = default;
    SpriteRendererComponent(const glm::vec4& color)
        : Color(color) {}
};
```

## Scene 作用域

```c++
///I recommend you to use the lastest version of entt.hpp
///Here is the lastest:https://github.com/skypjack/entt/tree/master/single_include/entt
///Otherwise you may encounter this issue.
///https://github.com/skypjack/entt/issues/96
#include "entt/entt.hpp"
Scene::Scene()
{
}

Scene::~Scene()
{
}

entt::entity Scene::CreateEntity()
{
    return m_Registry.create();
}

void Scene::OnUpdate(Timestep ts)
{
    auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
    for (auto entity : group)
    {
        //https://github.com/skypjack/entt/issues/96
        //wrong E0461:
        //auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
        auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
        Renderer2D::DrawQuad(transform, sprite.Color);
    }
}
```

## Render 2D 作用域

重构绘制面片**DrawQuad**方法，将变换部分（**transform**）分出一个方法

```c++
void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
{
    CC3D_PROFILE_FUNCTION();

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
        * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    DrawQuad(transform, color);
}

void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
{
    CC3D_PROFILE_FUNCTION();

    constexpr size_t quadVertexCount = 4;
    const float textureIndex = 0.0f; // White Texture
    constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
    const float tilingFactor = 1.0f;

    if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
        FlushAndReset();

    for (size_t i = 0; i < quadVertexCount; i++)
    {
        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
        s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_Data.QuadVertexBufferPtr++;
    }

    s_Data.QuadIndexCount += 6;

    s_Data.Stats.QuadCount++;
}
void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
{
    CC3D_PROFILE_FUNCTION();

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
        * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    DrawQuad(transform, texture, tilingFactor);
}
void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
{
    CC3D_PROFILE_FUNCTION();

    constexpr size_t quadVertexCount = 4;
    constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

    if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
        FlushAndReset();

    float textureIndex = 0.0f;
    for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
    {
        if (*s_Data.TextureSlots[i].get() == *texture.get())
        {
            textureIndex = (float)i;
            break;
        }
    }

    if (textureIndex == 0.0f)
    {
        if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
            FlushAndReset();

        textureIndex = (float)s_Data.TextureSlotIndex;
        s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
        s_Data.TextureSlotIndex++;
    }

    for (size_t i = 0; i < quadVertexCount; i++)
    {
        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
        s_Data.QuadVertexBufferPtr->Color = tintColor;
        s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
        s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_Data.QuadVertexBufferPtr++;
    }

    s_Data.QuadIndexCount += 6;

    s_Data.Stats.QuadCount++;
}

```
## OpenGL Frame buffer 作用域

**尝试解决视图随窗口均匀变化问题**

```c++
void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
{
    if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
    {
        CC3D_CORE_WARN("Attempted to rezize framebuffer to {0}, {1}", width, height);
        return;
    }
	//...code...
}
```

## CC3D Input 作用域

**现在我们的视图随场景（Scene）作用域更新**

```c++
Ref<Scene> m_ActiveScene;
entt::entity m_SquareEntity;
void EditorLayer::OnAttach()
{
    CC3D_PROFILE_FUNCTION();

    m_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");

    FramebufferSpecification fbSpec;
    fbSpec.Width = 1280;
    fbSpec.Height = 720;
    m_Framebuffer = Framebuffer::Create(fbSpec);

    ///Scene Component:
    m_ActiveScene = CreateRef<Scene>();

    auto square = m_ActiveScene->CreateEntity();
    m_ActiveScene->Reg().emplace<TransformComponent>(square);
    m_ActiveScene->Reg().emplace<SpriteRendererComponent>(square, glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });

    m_SquareEntity = square;
}
void EditorLayer::OnUpdate(Timestep ts)
{
    CC3D_PROFILE_FUNCTION();

    // Update
    if (m_ViewportFocused)
        m_CameraController.OnUpdate(ts);

    // Render
    Renderer2D::ResetStats();

    CC3D_PROFILE_SCOPE("Renderer Prep");
    m_Framebuffer->Bind();
    RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
    RenderCommand::Clear();

    Renderer2D::BeginScene(m_CameraController.GetCamera());
    // Update scene
    m_ActiveScene->OnUpdate(ts);

    Renderer2D::EndScene();
}
void EditorLayer::OnImGuiRender()
{
	//...code...

    auto& squareColor = m_ActiveScene->Reg().get<SpriteRendererComponent>(m_SquareEntity).Color;
    ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));

	//...code...
}
```

## 构建和结果

![results](./results.gif)