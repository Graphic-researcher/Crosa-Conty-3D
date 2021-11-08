# Lec55 Scene Viewport

tutorial : [here](https://www.youtube.com/watch?v=xiSW4UgjLKU&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=73)

code version :  [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/c38d29cdd9ab6c0f1e3355e74e2ae6d4892c32db/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D)

## Frame Buffer Scope

### Frame Buffer Class

```c++
virtual void Resize(uint32_t width, uint32_t height) = 0;
```

### OpenGL Frame Buffer Class

```c++
OpenGLFramebuffer::~OpenGLFramebuffer()
{
    glDeleteFramebuffers(1, &m_RendererID);
    glDeleteTextures(1, &m_ColorAttachment);
    glDeleteTextures(1, &m_DepthAttachment);
}

void OpenGLFramebuffer::Invalidate()
{
    if (m_RendererID)
    {
        glDeleteFramebuffers(1, &m_RendererID);
        glDeleteTextures(1, &m_ColorAttachment);
        glDeleteTextures(1, &m_DepthAttachment);
    }

	//...code...
}

void OpenGLFramebuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    glViewport(0, 0, m_Specification.Width, m_Specification.Height);
}
void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
{
    m_Specification.Width = width;
    m_Specification.Height = height;

    Invalidate();
}
```

## OrthographicCameraController Scope

```c++
void OrthographicCameraController::OnResize(float width, float height)
{
    m_AspectRatio = width / height;
    m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
}
```

## CC3D Input Scope

```c++
void EditorLayer::OnImGuiRender()
{
	//...code...

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
    ImGui::Begin("Viewport");
    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize))
    {
        m_Framebuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
        m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

        m_CameraController.OnResize(viewportPanelSize.x, viewportPanelSize.y);
    }
    uint32_t textureID = m_Framebuffer->GetColorAttachc++mentRendererID();
    ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
    ImGui::End();
    ImGui::PopStyleVar();

	//...code...
}

```

## Build and Result

![result](./result.gif)