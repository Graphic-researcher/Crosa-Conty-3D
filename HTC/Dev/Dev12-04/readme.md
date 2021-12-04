# Road to PBR!

## Environment Loading

```c++
struct FlagWithPath
{
    bool flag;//is drag a new texture
    std::string path;//texture path
    bool resetFlag = false;//is reset texture
};
void EditorLayer::EnvironmentLoad()
{
    ImGui::Begin("Environment");
    Ref<Texture2D>& HDRTexture = m_ActiveScene->m_Cubemap->GetTexture();
    FlagWithPath f = ShowSetTexture(HDRTexture, "HDR");
    if (f.flag)
    {
        m_ActiveScene->m_Cubemap = Cubemap::Create(f.path);
    }
    else if (f.resetFlag)
    {
        m_ActiveScene->m_Cubemap = Cubemap::Create();
    }
    ImGui::End();
}
static FlagWithPath ShowSetTexture(Ref<Texture2D>& Texture, const std::string& textureName)
{
    ImGui::Text((textureName + " Texture").c_str());

    static float thumbnailSize = 128.0f;
    FlagWithPath flagWithPath = {false,""};
    if (nullptr != Texture)
    {
        ImGui::ImageButton((ImTextureID)Texture->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
    }
    else
    {
        ImGui::Button(textureName.c_str(), ImVec2(100.0f, 0.0f));
    }

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
        {
            const wchar_t* path = (const wchar_t*)payload->Data;
            std::filesystem::path texturePath = std::filesystem::path(g_AssetPath) / path;
            Ref<Texture2D> texture = Texture2D::Create(texturePath.string());
            if (texture->IsLoaded())
            {
                Texture = texture;
                flagWithPath.flag = true;
                flagWithPath.path = texturePath.string();
            }
            else
                CC3D_WARN("Could not load texture {0}", texturePath.filename().string());
        }
        ImGui::EndDragDropTarget();
    }
    std::string btn = textureName + " Reset";
    if (ImGui::Button(btn.c_str(), ImVec2(100.0f, 0.0f)))
    {
        Texture = Texture2D::Create(1, 1);
        uint32_t data = 0xffffffff;
        Texture->SetData(&data, sizeof(uint32_t));
        flagWithPath.resetFlag = true;
    }
    return flagWithPath;
}
```

![image-20211204124737404](envload.png)

![skybox](skybox.gif)

