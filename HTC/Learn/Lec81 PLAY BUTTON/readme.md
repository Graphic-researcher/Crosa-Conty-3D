#  Lec 81 PLAY BUTTON

tutorial : [here](https://www.youtube.com/watch?v=U16wc8w8IA4&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=105)

code version : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/commit/c669ed0c1a9bf996d8ebe4ee1d8e81c8223b8b9c)

## CC3D Input Editor Layer

```c++
void OnScenePlay();
void OnSceneStop();

// UI Panels
void UI_Toolbar();
enum class SceneState
{
    Edit = 0, Play = 1
};
SceneState m_SceneState = SceneState::Edit;
// Editor resources
Ref<Texture2D> m_IconPlay, m_IconStop;
void EditorLayer::OnAttach()
{
    //...
    m_IconPlay = Texture2D::Create("Resources/Icons/PlayButton.png");
    m_IconStop = Texture2D::Create("Resources/Icons/StopButton.png");
    //...
}
void EditorLayer::OnUpdate(Timestep ts)
{
    //...
    switch (m_SceneState)
    {
        case SceneState::Edit:
            {
                if (m_ViewportFocused)
                    m_CameraController.OnUpdate(ts);

                m_EditorCamera.OnUpdate(ts);

                m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);
                break;
            }
        case SceneState::Play:
            {
                m_ActiveScene->OnUpdateRuntime(ts);
                break;
            }
    }
    //...
}
void EditorLayer::OnImGuiRender()
{
    //...
    UI_Toolbar();
    ImGui::End();
}
void EditorLayer::UI_Toolbar()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    auto& colors = ImGui::GetStyle().Colors;
    const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
    const auto& buttonActive = colors[ImGuiCol_ButtonActive];
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

    //ImGui::Begin("##toolbar");//do it first to find a proper place to fix then change to the follow line,otherwise you can change the toolbar with so many args in Imgui::Begin
    ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    float size = ImGui::GetWindowHeight() - 4.0f;
    Ref<Texture2D> icon = m_SceneState == SceneState::Edit ? m_IconPlay : m_IconStop;
    ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
    if (ImGui::ImageButton((ImTextureID)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0))
    {
        if (m_SceneState == SceneState::Edit)
            OnScenePlay();
        else if (m_SceneState == SceneState::Play)
            OnSceneStop();
    }
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(3);
    ImGui::End();
}
void EditorLayer::OnScenePlay()
{
    m_SceneState = SceneState::Play;
}

void EditorLayer::OnSceneStop()
{
    m_SceneState = SceneState::Edit;

}
```

## Build and Result

![result](./result.gif)