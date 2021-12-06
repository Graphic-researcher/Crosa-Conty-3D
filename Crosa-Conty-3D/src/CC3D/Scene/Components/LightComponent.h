#pragma once
#include "CC3D/Core/Base.h"
#include "CC3D/Renderer/Light.h"

namespace CC3D {

	struct LightComponent
	{
		Ref<Light> light;
		LightType Type = LightType::Point;

		LightComponent() { SetLightType(LightType::Point); }
		LightComponent(const LightComponent&) = default;
		LightComponent(LightType type) { 
			SetLightType(type);
		}
		
		void SetLightType(LightType type)
		{
			switch (type)
			{
			case LightType::Point:
				Type = LightType::Point;
				light = CreateRef<PointLight>();
				break;
			case LightType::Direct:
				Type = LightType::Direct;
				light = CreateRef<DirectLight>();
				break;
			case LightType::Spot:
				Type = LightType::Spot;
				light = CreateRef<SpotLight>();
				break;
			default:
				CC3D_CORE_ASSERT(false, "Not available light type!");
				break;
			}
		}


		void Bind(const Ref<Material>& material, const glm::vec3& position, const uint32_t& slot = 0)
		{
			light->Bind(material, position, slot);
		}
		void Bind(const Ref<Material>& material, const glm::vec3& position, const glm::vec3& rotation, const uint32_t& slot = 0)
		{
			light->Bind(material, position, rotation, slot);
		}

	};
}