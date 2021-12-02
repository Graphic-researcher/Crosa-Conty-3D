#pragma once
#include "CC3D/Renderer/Shader.h"

#include <glm/glm.hpp>
namespace CC3D {

	enum class LightType
	{
		Point = 0,
		Directional,
		Spot,
	};

	class Light
	{
	public:
		Light() = default;
		~Light() = default;

		virtual void Bind(const Ref<Material>& material, const glm::vec3& position, const uint32_t& slot) = 0;

		glm::vec3 Color = glm::vec3(1.0f, 1.0f, 1.0f);
		float Intensity = 1.0f;
	private:
		
	};

	class PointLight : public Light
	{
	public:
		PointLight() = default;
		~PointLight() = default;

		virtual void Bind(const Ref<Material>& material, const glm::vec3& position, const uint32_t& slot) override;

		float Range = 10.0f;
	private:
		float Constant = 1.0f;
		float Linear = 0.09f;
		float Quadratic = 0.032f;
	};
}