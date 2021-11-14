#include "cc3d_pch.h"
#include "CC3D/Scene/Entity.h"


namespace CC3D {

	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
	}

}