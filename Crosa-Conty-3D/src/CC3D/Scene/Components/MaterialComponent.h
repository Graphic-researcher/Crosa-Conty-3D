#pragma once

#include "CC3D/Renderer/Material.h"

namespace CC3D {

	struct MaterialComponent
	{
		Ref<Material> material;
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		MaterialComponent() { material = CreateRef<PhongMaterial>(); }
		MaterialComponent(MaterialType materialType) {
			switch (materialType)
			{
			case MaterialType::Phong:
				material = CreateRef<PhongMaterial>();
				break;
			default:
				break;
			}
		}
		MaterialComponent(const MaterialComponent&) = default;
	};
}