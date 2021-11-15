# Lec 62 Properties Panel

tutorial : [here](https://www.youtube.com/watch?v=NBpB0qscF3E&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=85)

code version : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/commit/7009a1caab9234cd0a6ad2d14b2c435680f02eaf)

## CC3D Input Scene Hierarchy Panel Scope

```c++
void SceneHierarchyPanel::OnImGuiRender()
{
    ImGui::Begin("Scene Hierarchy");

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

    ImGui::End();

    ImGui::Begin("Properties");
    if (m_SelectionContext)
        DrawComponents(m_SelectionContext);

    ImGui::End();
}
void SceneHierarchyPanel::DrawComponents(Entity entity)
{
    if (entity.HasComponent<TagComponent>())
    {
        auto& tag = entity.GetComponent<TagComponent>().Tag;

        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strcpy_s(buffer, sizeof(buffer), tag.c_str());
        if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
        {
            tag = std::string(buffer);
        }
    }

    if (entity.HasComponent<TransformComponent>())
    {
        if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
        {
            auto& transform = entity.GetComponent<TransformComponent>().Transform;
            ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);

            ImGui::TreePop();
        }
    }
}
```

## Build and Result

![results](./results.gif)

