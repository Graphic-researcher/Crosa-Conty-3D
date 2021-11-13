#include "ccpch.h"
#include "Entity.h"

namespace CC3D {

	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
	}

}