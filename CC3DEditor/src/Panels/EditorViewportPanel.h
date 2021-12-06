#pragma once
#include "CC3D/Core/Base.h"
#include "CC3D/Core/Log.h"
#include "CC3D/Scene/Scene.h"
#include "CC3D/Scene/Entity.h"
#include "CC3D/Renderer/Framebuffer.h"

namespace CC3D {

	class EditorViewportPanel
	{
	public:
		EditorViewportPanel() = default;
		EditorViewportPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);

		void OnImGuiRender();

		void SetFramebuffer(Ref<Framebuffer> framebuffer);

	private:
		void DrawGizmos();
	private:
		Ref<Framebuffer> m_Framebuffer;
		Ref<Scene> m_Context;

		glm::vec2 m_EditorViewportSize = glm::vec2{ 0.0f, 0.0f };
		glm::vec2 m_EditorViewportBounds[2];
	};

}
