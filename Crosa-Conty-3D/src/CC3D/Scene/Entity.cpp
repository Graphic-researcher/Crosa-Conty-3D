#include "ccpch.h"
#include "Entity.h"
#include "CC3D/Scene/Components/TransformComponent.h"
namespace CC3D {

	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
	}
	void Entity::AddSubEntity(Entity other)
	{
		// Entity 是通过entt的神秘id老寻址的，所以不需要用指针，用指针反而会出错
		other.GetComponent<TransformComponent>().parent = *this;
		this->GetComponent<TransformComponent>().children.emplace(other.GetUUID(),other);
	}

}