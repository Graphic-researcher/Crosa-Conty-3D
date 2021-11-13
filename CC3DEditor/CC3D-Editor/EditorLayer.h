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
		OrthographicCameraController m_CameraController;

		bool m_ViewportFocused = false, m_ViewportHovered = false;

		Ref<Texture2D> m_WaifuTexture;
		Ref<Texture2D> m_SAGATexture;
		Ref<Framebuffer> m_Framebuffer;

		Ref<Scene> m_ActiveScene;
		Entity m_SquareEntity;
		glm::vec4 m_SquareColor = glm::vec4{ 0.2f, 0.3f, 0.8f, 1.0f };

		glm::vec2 m_ViewportSize = glm::vec2{ 0.0f, 0.0f };
	};

}