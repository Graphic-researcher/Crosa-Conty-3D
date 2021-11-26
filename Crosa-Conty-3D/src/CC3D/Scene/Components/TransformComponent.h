#pragma once

#include "CC3D/Scene/Entity.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
namespace CC3D {

	struct TransformComponent
	{
		glm::vec3 Translation = glm::vec3{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = glm::vec3{ 0.0f, 0.0f, 0.0f }; // TODO Eular rotation
		glm::vec3 Scale = glm::vec3{ 1.0f, 1.0f, 1.0f };

		glm::vec3 GlobalTranform = glm::vec3{ 0.0f, 0.0f, 0.0f };
		glm::quat GlobalRotation = glm::quat{ 0.0f, 0.0f, 0.0f, 0.0f }; //TODO quternion rotation
		glm::vec3 GlobalScale = glm::vec3{ 1.0f, 1.0f, 1.0f }; // TODO GlobalScale
		Entity* parent = nullptr;
		Entity* child = nullptr;

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: GlobalTranform(translation) {}

		glm::mat4 GetGlobalTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), GlobalTranform)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
		void GetLocalFromGlobal()
		{
			if (parent)
			{
				Translation = GlobalTranform - parent->GetComponent<TransformComponent>().GlobalTranform;
				//Rotation = GlobalRotation - parent->GetComponent<TransformComponent>().GlobalRotation;
			}
			else
			{
				Translation = GlobalTranform;
			}

		}

	};
}