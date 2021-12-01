#pragma once

#include "CC3D/Renderer/Material.h"

namespace CC3D {

	struct MaterialComponent
	{
		Material material;
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		MaterialComponent() = default;
		MaterialComponent(const MaterialComponent&) = default;
	};
}