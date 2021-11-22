#pragma once

#include <CC3D.h>
#include "Panels/SceneHierarchyPanel.h"

#include "CC3D/Renderer/EditorCamera.h"

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

		Entity m_HoveredEntity;

		EditorCamera m_EditorCamera;

		glm::vec2 m_ViewportSize = glm::vec2{ 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];
		int m_GizmoType = -1;

		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
	};

}