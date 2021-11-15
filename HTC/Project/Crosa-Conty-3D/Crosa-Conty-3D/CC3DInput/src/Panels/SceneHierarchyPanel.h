#pragma once

#include "CC3D/Core/Core.h"
#include "CC3D/Core/Log.h"
#include "CC3D/Scene/Scene.h"
#include "CC3D/Scene/Entity.h"

namespace CC3D {

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);

		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};

}
