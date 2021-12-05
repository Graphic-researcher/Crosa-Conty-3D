#pragma once

#include <glm/glm.hpp>

namespace CC3D::Math {

	bool DecomposeTransform(const glm::mat4& transform, glm::vec3& Translation, glm::vec3& Rotation, glm::vec3& Scale);
	bool GetTranslation(const glm::mat4& transform, glm::vec3& translation);
}
