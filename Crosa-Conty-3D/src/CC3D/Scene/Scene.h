#pragma once

#include "entt.hpp"

#include "CC3D/Core/Timestep.h"

namespace CC3D {

	class Scene
	{
	public:
		Scene();
		~Scene();

		entt::entity CreateEntity();

		// TEMP
		entt::registry& Reg() { return m_Registry; }

		void OnUpdate(Timestep ts);
	private:
		entt::registry m_Registry;
	};

}