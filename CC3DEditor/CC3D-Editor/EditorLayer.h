#pragma once

#include <CC3D.h>

namespace CC3D {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;
	private:
		CC3D::OrthographicCameraController m_CameraController;

		bool m_ViewportFocused = false, m_ViewportHovered = false;

		CC3D::Ref<CC3D::Texture2D> m_WaifuTexture;
		CC3D::Ref<CC3D::Texture2D> m_SAGATexture;
		CC3D::Ref<CC3D::Framebuffer> m_Framebuffer;

		glm::vec2 m_ViewportSize = glm::vec2{ 0.0f, 0.0f };
	};

}