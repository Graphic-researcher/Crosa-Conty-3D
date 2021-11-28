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

		glm::vec3 GlobalTranslation = glm::vec3{ 0.0f, 0.0f, 0.0f };
		glm::vec3 GlobalRotation = glm::vec3{ 0.0f, 0.0f, 0.0f }; //TODO quternion rotation
		glm::vec3 GlobalScale = glm::vec3{ 1.0f, 1.0f, 1.0f }; // TODO GlobalScale

		Entity parent;
		std::vector<Entity> child;

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: GlobalTranslation(translation) {}

		// TODO need improve
		void UpdateGlobalTransform()
		{
			if (!this->parent)
			{
				GlobalTranslation = Translation;
				GlobalRotation = Rotation;
				GlobalScale = Scale;
			}	
			else
			{
				Entity p = this->parent;
				GlobalTranslation = Translation;
				GlobalRotation = Rotation;
				GlobalScale = Scale;
				glm::mat4 Local2Global = glm::mat4(1.0f);

				while (p) 
				{
					// TODO Change to Matrix perform
					GlobalTranslation += p.GetComponent<TransformComponent>().Translation;
					GlobalRotation += p.GetComponent<TransformComponent>().Rotation;
					GlobalScale *= p.GetComponent<TransformComponent>().Scale;
					//glm::mat4 transform = p->GetComponent<TransformComponent>().GetLocalTransform();
					//Local2Global = transform * Local2Global;
					p = p.GetComponent<TransformComponent>().parent;
				}
	/*			GlobalTranslation = Local2Global * glm::vec4(GlobalTranslation, 0.0f);
				GlobalRotation = Local2Global * glm::vec4(GlobalRotation, 0.0f);
				GlobalScale = Local2Global * glm::vec4(GlobalScale, 0.0f);*/
			}

			
		}

		glm::mat4 GetGlobalTransform()
		{
			UpdateGlobalTransform();
			glm::mat4 g_Rotation = glm::toMat4(glm::quat(GlobalRotation));

			return glm::translate(glm::mat4(1.0f), GlobalTranslation)
				* g_Rotation
				* glm::scale(glm::mat4(1.0f), GlobalScale);
		}
		glm::mat4 GetLocalTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
		
		
	};
}