#pragma once
#include "CC3D/Core/Base.h"
#include "CC3D/Core/Log.h"
#include "CC3D/Scene/Scene.h"
#include "CC3D/Scene/Entity.h"
#include "CC3D/Renderer/Framebuffer.h"

namespace CC3D {

	class GameViewportPanel
	{
	public:
		GameViewportPanel() = default;
		GameViewportPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);

		void OnImGuiRender();

		void SetFramebuffer(Ref<Framebuffer> framebuffer);

		Entity GetSelectedEntity() const { return m_SelectionContext; }
		void SetSelectedEntity(Entity entity);

	private:
		void DrawGizmos();// TODO ‘⁄Game ”Õºø™∆ÙGizmos
	private:
		Ref<Framebuffer> m_Framebuffer;
		Ref<Scene> m_Context;
		Entity m_SelectionContext;

		glm::vec2 m_GameViewportSize = glm::vec2{ 0.0f, 0.0f };
		glm::vec2 m_GameViewportBounds[2];
	};

}
