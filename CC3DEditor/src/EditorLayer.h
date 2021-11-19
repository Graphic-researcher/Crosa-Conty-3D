#pragma once

#include <CC3D.h>
#include "Panels/SceneHierarchyPanel.h"

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
		bool OnKeyPressed(KeyPressedEvent& e);

		void NewScene();
		void OpenScene();
		void SaveSceneAs();
	private:
		OrthographicCameraController m_CameraController;

		bool m_ViewportFocused = false, m_ViewportHovered = false;

		Ref<Texture2D> m_WaifuTexture;
		Ref<Texture2D> m_SAGATexture;
		Ref<Framebuffer> m_Framebuffer;

		Ref<Scene> m_ActiveScene;
		Entity m_SquareEntity;
		Entity m_CameraEntity;
		Entity m_SecondCamera;
		Entity m_ParticleSystem;

		bool m_PrimaryCamera = true;

		glm::vec4 m_SquareColor = glm::vec4{ 0.2f, 0.3f, 0.8f, 1.0f };

		glm::vec2 m_ViewportSize = glm::vec2{ 0.0f, 0.0f };

		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
	};

}