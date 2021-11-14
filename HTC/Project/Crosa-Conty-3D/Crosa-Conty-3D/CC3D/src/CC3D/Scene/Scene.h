#pragma once
///I recommend you to use the lastest version of entt.hpp
///Here is the lastest:https://github.com/skypjack/entt/tree/master/single_include/entt
///Otherwise you may encounter this issue.
///https://github.com/skypjack/entt/issues/96
#include "entt/entt.hpp"


#include "CC3D/Core/Timestep.h"

namespace CC3D {
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());

		void OnUpdate(Timestep ts);
	private:
		entt::registry m_Registry;
		friend class Entity;
	};

}