#pragma once
#include "CC3D/Core/Base.h"
#include "CC3D/Renderer/Light.h"

namespace CC3D {

	struct LightComponent
	{
		Ref<Light> light;
		LightType Type = LightType::Point;

		LightComponent() { light = CreateRef<PointLight>(); }
		LightComponent(LightType type) { 
			switch (type)
			{
			case LightType::Point:
				Type = LightType::Point;
				light = CreateRef<PointLight>();
				break;
			}
		}
		LightComponent(const LightComponent&) = default;

		void Bind(const Ref<Material>& material, const glm::vec3& position, const uint32_t& slot = 0)
		{
			switch (Type)
			{
			case LightType::Point:
				light->Bind(material, position, slot);
				break;
			default:
				break;
			}
		}

	};
}