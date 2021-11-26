#pragma once
///I recommend you to use the lastest version of entt.hpp
///Here is the lastest:https://github.com/skypjack/entt/tree/master/single_include/entt
///Otherwise you may encounter this issue.
///https://github.com/skypjack/entt/issues/96

#include "CC3D/Core/Timestep.h"
#include "CC3D/Renderer/EditorCamera.h"
#include "CC3D/Core/UUID.h"

#include "entt/entt.hpp"

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

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		
		b2World* m_PhysicsWorld = nullptr;
		
		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};

}