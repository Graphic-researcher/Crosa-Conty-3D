#pragma once
#include <filesystem>

#include "CC3D/Core/Core.h"
#include "CC3D/Core/Log.h"
#include "CC3D/Scene/Scene.h"
#include "CC3D/Scene/Entity.h"

namespace CC3D {

	struct FlagWithPath
	{
		bool flag;//is drag a new texture
		std::string path;//texture path
		bool resetFlag = false;//is reset texture
	};

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);

		void OnImGuiRender();

		Entity GetSelectedEntity() const { return m_SelectionContext; }
		void SetSelectedEntity(Entity entity);

	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};
	static FlagWithPath ShowSetTexture(Ref<Texture2D>& Texture, const std::string& textureName);
}
