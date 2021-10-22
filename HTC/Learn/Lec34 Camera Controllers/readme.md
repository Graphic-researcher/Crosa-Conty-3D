# Lec34 Camera Controllers

tutorial : [here](https://www.youtube.com/watch?v=q7rwMoZQSmw&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=47)

code version : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/507411b18be783b9902ea670c456ebe4748f75eb/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D)

prerequiste : [2D Renderer](https://www.youtube.com/watch?v=ULO8JhM9FsQ&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=46) [Projection](https://www.youtube.com/watch?v=xZs6K7VLM7A&list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2&index=20)

## Camera Scope

### Camera 

```c++
void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
{
    m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}
```

### Camera Controller

```c++
class OrthographicCameraController
{
public:
	OrthographicCameraController(float aspectRatio, bool rotation = false);

	void OnUpdate(Timestep ts);
	void OnEvent(Event& e);

	OrthographicCamera& GetCamera() { return m_Camera; }
	const OrthographicCamera& GetCamera() const { return m_Camera; }
private:
	bool OnMouseScrolled(MouseScrolledEvent& e);
	bool OnWindowResized(WindowResizeEvent& e);
private:
	float m_AspectRatio;
	float m_ZoomLevel = 1.0f;
	OrthographicCamera m_Camera;

	bool m_Rotation;

	glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
	float m_CameraRotation = 0.0f;
	float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 180.0f;
};
OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
	: m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
{
}

void OrthographicCameraController::OnUpdate(Timestep ts)
{
	if (Input::IsKeyPressed(CC3D_KEY_A))
		m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
	else if (Input::IsKeyPressed(CC3D_KEY_D))
		m_CameraPosition.x += m_CameraTranslationSpeed * ts;

	if (Input::IsKeyPressed(CC3D_KEY_W))
		m_CameraPosition.y += m_CameraTranslationSpeed * ts;
	else if (Input::IsKeyPressed(CC3D_KEY_S))
		m_CameraPosition.y -= m_CameraTranslationSpeed * ts;

	if (m_Rotation)
	{
		if (Input::IsKeyPressed(CC3D_KEY_Q))
			m_CameraRotation += m_CameraRotationSpeed * ts;
		if (Input::IsKeyPressed(CC3D_KEY_E))
			m_CameraRotation -= m_CameraRotationSpeed * ts;

		m_Camera.SetRotation(m_CameraRotation);
	}

	m_Camera.SetPosition(m_CameraPosition);

	m_CameraTranslationSpeed = m_ZoomLevel;
}

void OrthographicCameraController::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<MouseScrolledEvent>(CC3D_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
	dispatcher.Dispatch<WindowResizeEvent>(CC3D_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
}

bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
{
	m_ZoomLevel -= e.GetYOffset() * 0.25f;
	m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
	m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	return false;
}

bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
{
	m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
	m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	return false;
}
```

## SandBox Scope

```c++
void OnUpdate(CC3D::Timestep ts) override
{
    ///Log the delta time
  	//...

    // Update
    m_CameraController.OnUpdate(ts);

    ///model transform
	//...

	//...render
}
void OnEvent(CC3D::Event& e) override
{
	m_CameraController.OnEvent(e);
}
```

## Build and Result

You can zoom in/out with mouse scroll

![cameracontrol](./cameracontrol.gif)