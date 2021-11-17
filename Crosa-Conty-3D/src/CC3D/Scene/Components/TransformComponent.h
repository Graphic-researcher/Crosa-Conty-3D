#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
namespace CC3D {

	struct TransformComponent
	{
		glm::vec3 Translation = glm::vec3{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = glm::vec3{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = glm::vec3{ 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation) {}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, glm::vec3{ 1, 0, 0 })
				* glm::rotate(glm::mat4(1.0f), Rotation.y, glm::vec3{ 0, 1, 0 })
				* glm::rotate(glm::mat4(1.0f), Rotation.z, glm::vec3{ 0, 0, 1 });

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}

	};
}