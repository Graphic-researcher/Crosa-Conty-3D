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

	CC3D::Ref<CC3D::Texture2D> m_WaifuTexture;
	CC3D::Ref<CC3D::Texture2D> m_SAGATexture;
	CC3D::Ref<CC3D::Framebuffer> m_Framebuffer;
};