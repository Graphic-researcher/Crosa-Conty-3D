# Lec64 Drawing Component UI

tutorial : [here](https://www.youtube.com/watch?v=u3yq8s3KuSE&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=87)

code version : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/commit/59e9c070f94075f04ac90903f90bfca754393a93)

## CC3D Input Scene Hierarchy Panel Scope

```c++
void SceneHierarchyPanel::DrawComponents(Entity entity)
{
    //...code

    if (entity.HasComponent<SpriteRendererComponent>())
    {
        if (ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sprite Renderer"))
        {
            auto& src = entity.GetComponent<SpriteRendererComponent>();
            ImGui::ColorEdit4("Color", glm::value_ptr(src.Color));
            ImGui::TreePop();
        }
    }
}
```

## Build and Result

![color1](./color1.png)

![color2](./color2.png)