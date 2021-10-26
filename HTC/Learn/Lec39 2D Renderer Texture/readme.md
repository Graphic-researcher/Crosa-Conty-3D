# Lec39 2D Renderer Texture

tutorial : [here](https://www.youtube.com/watch?v=mW8eW3pLtmk&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=53)

code version :











```c++
CC3D::Renderer2D::BeginScene(m_CameraController.GetCamera());
CC3D::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
CC3D::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });	
CC3D::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
CC3D::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, m_CheckerboardTexture);
CC3D::Renderer2D::EndScene();
```

![image-20211026162229959](https://i.loli.net/2021/10/26/7Z2gyfpxQXW9IhH.png)

```c++
CC3D::Renderer2D::BeginScene(m_CameraController.GetCamera());
CC3D::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, m_CheckerboardTexture);
CC3D::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
CC3D::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });	
CC3D::Renderer2D::EndScene();
```

![image-20211026162505289](https://i.loli.net/2021/10/26/TUElCSjJIq6oGBb.png)

use z coordinate to correct the depth:

```c++
CC3D::Renderer2D::BeginScene(m_CameraController.GetCamera());
CC3D::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
CC3D::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
CC3D::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
CC3D::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 1.0f, 1.0f }, m_CheckerboardTexture);
CC3D::Renderer2D::EndScene();
```

![image-20211026162651402](https://i.loli.net/2021/10/26/UtG8eqmRLQ4Z1uX.png)