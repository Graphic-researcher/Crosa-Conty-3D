# Lec 84 Playing and Stopping Scenes (and Resetting)

tutorial : [here](https://www.youtube.com/watch?v=EAC6_h07izo&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=108)

code version : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/commit/f19caf6dcca28b2be8d646c9cd6682bf06a9fbe4)

## Entity Scope

```c++
template<typename T, typename... Args>
T& AddOrReplaceComponent(Args&&... args)
{
    T& component = m_Scene->m_Registry.emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
    m_Scene->OnComponentAdded<T>(*this, component);
    return component;
}
const std::string& GetName() { return GetComponent<TagComponent>().Tag; }
```

## Scene Scope

```c++
template<typename Component>
static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
{
    auto view = src.view<Component>();
    for (auto e : view)
    {
        UUID uuid = src.get<IDComponent>(e).ID;
        CC3D_CORE_ASSERT(enttMap.find(uuid) != enttMap.end(),"uuid not equal!");
        entt::entity dstEnttID = enttMap.at(uuid);

        auto& component = src.get<Component>(e);
        dst.emplace_or_replace<Component>(dstEnttID, component);
    }
}

template<typename Component>
static void CopyComponentIfExists(Entity dst, Entity src)
{
    if (src.HasComponent<Component>())
        dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
}

Ref<Scene> Scene::Copy(Ref<Scene> other)
{
    Ref<Scene> newScene = CreateRef<Scene>();
    newScene->m_ViewportWidth = other->m_ViewportWidth;
    newScene->m_ViewportHeight = other->m_ViewportHeight;

    auto& srcSceneRegistry = other->m_Registry;
    auto& dstSceneRegistry = newScene->m_Registry;
    std::unordered_map<UUID, entt::entity> enttMap;

    // Create entities in new scene
    auto idView = srcSceneRegistry.view<IDComponent>();
    for (auto e : idView)
    {
        UUID uuid = srcSceneRegistry.get<IDComponent>(e).ID;
        const auto& name = srcSceneRegistry.get<TagComponent>(e).Tag;
        Entity newEntity = newScene->CreateEntityWithUUID(uuid, name);
        enttMap[uuid] = (entt::entity)newEntity;
    }

    // Copy components (except IDComponent and TagComponent)
    CopyComponent<TransformComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
    CopyComponent<SpriteRendererComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
    CopyComponent<CameraComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
    CopyComponent<NativeScriptComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
    CopyComponent<Rigidbody2DComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
    CopyComponent<BoxCollider2DComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);

    return newScene;
}
void Scene::DuplicateEntity(Entity entity)
{
    std::string name = entity.GetName();
    Entity newEntity = CreateEntity(name);

    CopyComponentIfExists<TransformComponent>(newEntity, entity);
    CopyComponentIfExists<SpriteRendererComponent>(newEntity, entity);
    CopyComponentIfExists<CameraComponent>(newEntity, entity);
    CopyComponentIfExists<NativeScriptComponent>(newEntity, entity);
    CopyComponentIfExists<Rigidbody2DComponent>(newEntity, entity);
    CopyComponentIfExists<BoxCollider2DComponent>(newEntity, entity);
}
```

## CC3D Input Scope

### Scene Hierarchy Panel

```c++
void SceneHierarchyPanel::OnImGuiRender()
{
    ImGui::Begin("Scene Hierarchy");

    if (m_Context)//we have entity to draw
    {
        m_Context->m_Registry.each([&](auto entityID)
                                   {
                                       Entity entity{ entityID , m_Context.get() };
                                       DrawEntityNode(entity);
                                   });

        /// <summary>
        /// If you don't select anything,select context will be none
        /// </summary>
        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
            m_SelectionContext = {};

        // Right-click on blank space
        if (ImGui::BeginPopupContextWindow(0, 1, false))
        {
            if (ImGui::MenuItem("Create Empty Entity"))
                m_Context->CreateEntity("Empty Entity");

            ImGui::EndPopup();
        }
    }
    //...
}
```

### Editor Layer

```c++
Ref<Scene> m_EditorScene;
std::filesystem::path m_EditorScenePath;
void EditorLayer::OnAttach()
{
    //...	
    m_EditorScene = m_ActiveScene;
    m_SceneHierarchyPanel.SetContext(m_EditorScene);
}
void EditorLayer::UI_Toolbar()
{
    //...
    if (ImGui::ImageButton((ImTextureID)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0))
    {
        if (m_SceneState == SceneState::Edit && m_EditorScene != nullptr)
            OnScenePlay();
        else if (m_SceneState == SceneState::Play)
            OnSceneStop();
    }
    //...
}
bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
{
    //...
    switch (e.GetKeyCode())
    {
            //...
        case Key::S:
            {
                if (control)
                {
                    if (shift)
                        SaveSceneAs();
                    else
                        SaveScene();
                }
                break;
            }
            // Scene Commands
        case Key::D:
            {
                if (control)
                    OnDuplicateEntity();

                break;
            }
            //...
    }
}
void EditorLayer::NewScene()
{
    //...
    m_EditorScenePath = std::filesystem::path();
}
void EditorLayer::OpenScene(const std::filesystem::path& path)
{
    if (m_SceneState != SceneState::Edit)
        OnSceneStop();
    //...
    if (serializer.Deserialize(path.string()))
    {
        m_EditorScene = newScene;
        m_EditorScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        m_SceneHierarchyPanel.SetContext(m_EditorScene);

        m_ActiveScene = m_EditorScene;
        m_EditorScenePath = path;
    }
}
void EditorLayer::SaveScene()
{
    if (!m_EditorScenePath.empty())
        SerializeScene(m_ActiveScene, m_EditorScenePath);
    else
        SaveSceneAs();
}
void EditorLayer::SaveSceneAs()
{
    std::string filepath = FileDialogs::SaveFile("CC3D Scene (*.cc3d)\0*.cc3d\0");
    if (!filepath.empty())
    {
        SerializeScene(m_ActiveScene, filepath);
        m_EditorScenePath = filepath;
    }
}
void EditorLayer::OnScenePlay()
{
    m_SceneState = SceneState::Play;

    m_ActiveScene = Scene::Copy(m_EditorScene);
    m_ActiveScene->OnRuntimeStart();
    m_SceneHierarchyPanel.SetContext(m_ActiveScene);
}
void EditorLayer::OnSceneStop()
{
    m_SceneState = SceneState::Edit;
    m_ActiveScene->OnRuntimeStop();
    m_ActiveScene = m_EditorScene;

    m_SceneHierarchyPanel.SetContext(m_ActiveScene);
}
void EditorLayer::SerializeScene(Ref<Scene> scene, const std::filesystem::path& path)
{
    SceneSerializer serializer(scene);
    serializer.Serialize(path.string());
}
void EditorLayer::OnDuplicateEntity()
{
    if (m_SceneState != SceneState::Edit)
        return;

    Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
    if (selectedEntity)
        m_EditorScene->DuplicateEntity(selectedEntity);
}
```

## Build and Result

Control S - save the scene directly

Control D - duplicate the entity

![result](./result.gif)

