#pragma once

#include "CC3D.h"

class Sandbox2D : public CC3D::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(CC3D::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(CC3D::Event& e) override;

private:
	CC3D::OrthographicCameraController m_CameraController;

	// Temp
	CC3D::Ref<CC3D::VertexArray> m_SquareVA;
	CC3D::ShaderLibrary m_ShaderLibrary;

	CC3D::Ref<CC3D::Texture2D> m_CheckerboardTexture;

	glm::vec4 m_SquareColor = glm::vec4(0.2f, 0.3f, 0.8f, 1.0f);

	// ImGui parameter
	bool TexCheck = true;
};