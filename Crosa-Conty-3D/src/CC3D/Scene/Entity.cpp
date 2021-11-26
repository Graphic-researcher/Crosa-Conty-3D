#include "ccpch.h"
#include "Entity.h"
#include "CC3D/Scene/Components/TransformComponent.h"
namespace CC3D {

	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
	}
	void Entity::AddSubEntity(Entity& other)
	{
		other.GetComponent<TransformComponent>().parent = this;
		this->GetComponent<TransformComponent>().child = &other;
	}

}