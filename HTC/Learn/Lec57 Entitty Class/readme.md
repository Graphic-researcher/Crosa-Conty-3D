# Lec 57 Entity Class

tutorial : [here](https://www.youtube.com/watch?v=GfSzeAcsBb0&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=79)

code version : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/2989df0ceba1794963e4959a54c89cbdd62373cd/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D)

## Component Scope

```c++
struct TagComponent
{
    std::string Tag;

    TagComponent() = default;
    TagComponent(const TagComponent&) = default;
    TagComponent(const std::string& tag)
        : Tag(tag) {}
};
```

## Entity Scope

```c++
class Entity
{
    public:
    Entity() = default;
    Entity(entt::entity handle, Scene* scene);
    Entity(const Entity& other) = default;

    template<typename T, typename... Args>
    T& AddComponent(Args&&... args)
    {
        CC3D_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
        return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
    }

    template<typename T>
    T& GetComponent()
    {
        CC3D_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
        return m_Scene->m_Registry.get<T>(m_EntityHandle);
    }

    template<typename T>
    bool HasComponent()
    {
        //return m_Scene->m_Registry.has<T>(m_EntityHandle);
        ///has method has been removed in the lastest version,so we use any_of
        return m_Scene->m_Registry.any_of<T>(m_EntityHandle);
    }

    template<typename T>
    void RemoveComponent()
    {
        CC3D_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
        m_Scene->m_Registry.remove<T>(m_EntityHandle);
    }

    operator bool() const { return m_EntityHandle != entt::null; }
    private:
    /// <summary>
    /// Fixed invalid entt::entity equal entt::null, which is not equal to 0 (#285)
    ///See https ://github.com/skypjack/entt/wiki/Crash-Course:-entity-component-system#null-entity for more details.
    /// </summary>
    entt::entity m_EntityHandle{ entt::null };
    Scene* m_Scene = nullptr;
};
Entity::Entity(entt::entity handle, Scene* scene)
    : m_EntityHandle(handle), m_Scene(scene)
    {
    }
```

## Scene Scope

```c++
class Entity;
class Scene
{
    public:
    Scene();
    ~Scene();

    Entity CreateEntity(const std::string& name = std::string());

    void OnUpdate(Timestep ts);
    private:
    entt::registry m_Registry;

    friend class Entity;
};
Entity Scene::CreateEntity(const std::string& name)
{
    Entity entity = { m_Registry.create(), this };
    entity.AddComponent<TransformComponent>();
    auto& tag = entity.AddComponent<TagComponent>();
    tag.Tag = name.empty() ? "Entity" : name;
    return entity;
}
```

## CC3D Input Scope

```c++
void EditorLayer::OnAttach()
{
    //...code...

    // Entity
    auto square = m_ActiveScene->CreateEntity("Green Square");
    square.AddComponent<SpriteRendererComponent>(glm::vec4{0.0f, 1.0f, 0.0f, 1.0f});

    m_SquareEntity = square;
}
void EditorLayer::OnImGuiRender()
{
	//...code...

    if (m_SquareEntity)
    {
        ImGui::Separator();
        auto& tag = m_SquareEntity.GetComponent<TagComponent>().Tag;
        ImGui::Text("%s", tag.c_str());

        auto& squareColor = m_SquareEntity.GetComponent<SpriteRendererComponent>().Color;
        ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));
        ImGui::Separator();
    }

	//...code...
}
```

