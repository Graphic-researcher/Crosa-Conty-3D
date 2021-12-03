# Annoying trouble -- Draw Cube map environment properly

Now I encounter a trouble which is made by MRT (Multiple Render Target)

If I use this frame buffer attachments:

```c++
fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };//can't load hdr
```

 The Environment Cube Map can't render properly :

![F](Buggy.gif)

Firstly I thought the problem is in view matrix :

```c++
void OpenGLCubemap::BindCubeMap(EditorCamera& camera, uint32_t slot) const
{
    m_BackgroundShader->Bind();
    m_BackgroundShader->SetMat4("projection", camera.GetProjectionMatrix());
    m_BackgroundShader->SetMat4("view", camera.GetViewMatrix());
    glBindTextureUnit(slot, m_CubemapID);
    RenderCube();
}
```

But when I delete the Depth attachment :

```c++
fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };//cant load hdr
```

The Cube Map could render good relatively

```c++
fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER};//could load hdr
```

![Normal](Normal.gif)

But the entity object disappeared (I guess that because the lost of depth)



