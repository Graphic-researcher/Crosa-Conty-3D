#pragma once
///I recommend you to use the lastest version of entt.hpp
///Here is the lastest:https://github.com/skypjack/entt/tree/master/single_include/entt
///Otherwise you may encounter this issue.
///https://github.com/skypjack/entt/issues/96

#include "CC3D/Core/Timestep.h"
#include "CC3D/Core/UUID.h"

#include "entt/entt.hpp"

#include "CC3D/Renderer/EditorCamera.h"
#include "CC3D/Renderer/Shader.h"
#include "CC3D/Renderer/3Ddev/effect.h"

class b2World;

namespace CC3D {
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		static Ref<Scene> Copy(Ref<Scene> other);

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());

		void DestroyEntity(Entity entity);

		void OnRuntimeStart();
		void OnRuntimeStop();

		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts, EditorCamera& camera);

		void OnViewportResize(uint32_t width, uint32_t height);
		
		void DuplicateEntity(Entity entity);
		Entity GetPrimaryCameraEntity();

		//TODO:3D Render Dev
		void RenderObject(EditorCamera& camera, const Ref<Cubemap>& envMap);
		void RenderLight(const EditorCamera& camera);
		//Environement:
		Ref<Cubemap> m_Cubemap = nullptr;
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		
		b2World* m_PhysicsWorld = nullptr;
		
		//TODO : 3D Render Dev
		Ref<Shader> m_DefaultShader;
		bool m_LineMode = false;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};

}