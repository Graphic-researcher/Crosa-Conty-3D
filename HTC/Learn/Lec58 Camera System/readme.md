# Lec58 Camera System

tutorial : [here](https://www.youtube.com/watch?v=ubZn7BlrnTU&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=80)

code version : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/dda03c8d667ce42534464374043f6ba87bee745b/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D) [implement version](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/a5d551ef50aceeac461b80f10621b0bcfee60844/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D)

## Camera Scope

```c++
class Camera
{
    public:
    Camera(const glm::mat4& projection)
        : m_Projection(projection) {}

    const glm::mat4& GetProjection() const { return m_Projection; }
    private:
    glm::mat4 m_Projection;
};
```

## Render 2D Scope

```c++
void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
{
    CC3D_PROFILE_FUNCTION();

    glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

    s_Data.TextureShader->Bind();
    s_Data.TextureShader->SetMat4("u_ViewProjection", viewProj);

    s_Data.QuadIndexCount = 0;
    s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

    s_Data.TextureSlotIndex = 1;
}
```

## Component Scope

```c++
struct CameraComponent
{
    CC3D::Camera Camera;
    /// <summary>
    /// "Primary" decide which camera is main camera that should render to viewport
    /// </summary>
    bool Primary = true; // TODO: think about moving to Scene

    CameraComponent() = default;
    CameraComponent(const CameraComponent&) = default;
    CameraComponent(const glm::mat4& projection)
        : Camera(projection) {}
};
```

## Scene Scope

```c++
void Scene::OnUpdate(Timestep ts)
{
    // Render 2D
    Camera* mainCamera = nullptr;
    glm::mat4* cameraTransform = nullptr;
    {
        auto group = m_Registry.view<TransformComponent, CameraComponent>();
        for (auto entity : group)
        {
            auto [transform, camera] = group.get<TransformComponent, CameraComponent>(entity);

            if (camera.Primary)
            {
                mainCamera = &camera.Camera;
                cameraTransform = &transform.Transform;
                break;
            }
        }
    }///cameraTransform 

    if (mainCamera)
    {
        Renderer2D::BeginScene(mainCamera->GetProjection(), *cameraTransform);

        auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for (auto entity : group)
        {
            //https://github.com/skypjack/entt/issues/96
            //wrong E0461:
            //auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
            auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
            Renderer2D::DrawQuad(transform, sprite.Color);
        }
        Renderer2D::EndScene();
    }///if (mainCamera)
}
```

## CC3D Input Scope

```c++
Ref<Scene> m_ActiveScene;
Entity m_SquareEntity;
Entity m_CameraEntity;
Entity m_SecondCamera;
bool m_PrimaryCamera = true;//decide main camera
void EditorLayer::OnAttach()
{
    //...code...

    m_ActiveScene = CreateRef<Scene>();

    // Entity
    auto square = m_ActiveScene->CreateEntity("Green Square");
    square.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });

    m_SquareEntity = square;

    m_CameraEntity = m_ActiveScene->CreateEntity("Camera Entity");
    m_CameraEntity.AddComponent<CameraComponent>(glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));

    m_SecondCamera = m_ActiveScene->CreateEntity("Clip-Space Entity");
    auto& cc = m_SecondCamera.AddComponent<CameraComponent>(glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f));
    cc.Primary = false;
}
void EditorLayer::OnImGuiRender()
{
    if (m_CameraEntity.GetComponent<CameraComponent>().Primary)
    {
        ImGui::DragFloat3("Camera Transform",
                          glm::value_ptr(m_CameraEntity.GetComponent<TransformComponent>().Transform[3]));
    }
    if (m_SecondCamera.GetComponent<CameraComponent>().Primary)
    {
        ImGui::DragFloat3("Second Camera Transform",
                          glm::value_ptr(m_SecondCamera.GetComponent<TransformComponent>().Transform[3]));
    }

    if (ImGui::Checkbox("Main Camera", &m_PrimaryCamera))
    {
        m_CameraEntity.GetComponent<CameraComponent>().Primary = m_PrimaryCamera;
        m_SecondCamera.GetComponent<CameraComponent>().Primary = !m_PrimaryCamera;
    }
}
```

## Build and Result

![results](./results.gif)