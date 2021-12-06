#pragma once
#include "CC3D/Renderer/Shader.h"
#include "CC3D/Renderer/Material.h"

#include <glm/glm.hpp>
namespace CC3D {

	enum class LightType
	{
		Point = 0,
		Direct,
		Spot,
	};

	struct Light
	{
		glm::vec3 Color = glm::vec3(1.0f, 1.0f, 1.0f);
		float Intensity = 1.0f;

		Light() = default;
		~Light() = default;

		virtual void Bind(const Ref<Material>& material, const glm::vec3& position, const uint32_t& slot) = 0;
		// For direct light and spot light
		virtual void Bind(const Ref<Material>& material, const glm::vec3& position, const glm::vec3& rotation, const uint32_t& slot) = 0;
	};

	struct PointLight : public Light
	{
		float Range = 10.0f;
		float Constant = 1.0f;
		float Linear = 0.09f;
		float Quadratic = 0.032f;

		PointLight() = default;
		~PointLight() = default;

		virtual void Bind(const Ref<Material>& material, const glm::vec3& position, const uint32_t& slot) override
		{
			material->SetFloat3("u_PointLight[" + std::to_string(slot) + "].color", Color);
			material->SetFloat3("u_PointLight[" + std::to_string(slot) + "].position", position);
			material->SetFloat("u_PointLight[" + std::to_string(slot) + "].constant", Constant);
			material->SetFloat("u_PointLight[" + std::to_string(slot) + "].linear", Linear);
			material->SetFloat("u_PointLight[" + std::to_string(slot) + "].quadratic", Quadratic);
			material->SetFloat("u_PointLight[" + std::to_string(slot) + "].intensity", Intensity);
		}

		virtual void Bind(const Ref<Material>& material, const glm::vec3& position, const glm::vec3& rotation, const uint32_t& slot) override
		{
			material->SetFloat3("u_PointLight[" + std::to_string(slot) + "].color", Color);
			material->SetFloat3("u_PointLight[" + std::to_string(slot) + "].position", position);
			material->SetFloat("u_PointLight[" + std::to_string(slot) + "].constant", Constant);
			material->SetFloat("u_PointLight[" + std::to_string(slot) + "].linear", Linear);
			material->SetFloat("u_PointLight[" + std::to_string(slot) + "].quadratic", Quadratic);
			material->SetFloat("u_PointLight[" + std::to_string(slot) + "].intensity", Intensity);
		}
	};

	struct DirectLight : public Light
	{
		glm::vec3 Direction{ 1.0f };

		DirectLight() = default;
		~DirectLight() = default;

		virtual void Bind(const Ref<Material>& material, const glm::vec3& position, const uint32_t& slot) override
		{
			material->Bind();
			material->SetFloat3("u_DirLight[" + std::to_string(slot) + "].color", Color);
			material->SetFloat3("u_DirLight[" + std::to_string(slot) + "].direction", Direction);
			material->SetFloat("u_DirLight[" + std::to_string(slot) + "].intensity", Intensity);
		}

		virtual void Bind(const Ref<Material>& material, const glm::vec3& position, const glm::vec3& rotation, const uint32_t& slot) override
		{
			material->Bind();
			material->SetFloat3("u_DirLight[" + std::to_string(slot) + "].color", Color);
			material->SetFloat3("u_DirLight[" + std::to_string(slot) + "].direction", rotation);
			material->SetFloat("u_DirLight[" + std::to_string(slot) + "].intensity", Intensity);
		}
	};

	struct SpotLight : public Light
	{
		SpotLight() = default;
		~SpotLight() = default;

		virtual void Bind(const Ref<Material>& material, const glm::vec3& position, const uint32_t& slot) override
		{
			int a = 0;
		}
		virtual void Bind(const Ref<Material>& material, const glm::vec3& position, const glm::vec3& rotation, const uint32_t& slot) override
		{
			int a = 0;
		}
	};
}