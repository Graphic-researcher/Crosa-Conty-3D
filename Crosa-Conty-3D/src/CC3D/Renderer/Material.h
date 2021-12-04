#pragma once
#include "CC3D/Renderer/Shader.h"
#include "CC3D/Renderer/Texture.h"

namespace CC3D 
{
	enum class MaterialType
	{
		Phong = 0,
		// TODO
		Lambert,
		BlinnPhong,
		Cook_Torrance,
	};

	struct Material
	{
		Ref<Shader> shader;
		Material() = default;
		~Material() = default;

		void Bind() { shader->Bind(); }
		void Unbind() { shader->Unbind(); }
		void SetInt(const std::string& name, int value) { shader->SetInt(name, value); }
		void SetIntArray(const std::string& name, int* values, uint32_t count) { shader->SetIntArray(name, values, count); }
		void SetFloat(const std::string& name, float value) { shader->SetFloat(name, value); }
		void SetFloat2(const std::string& name, const glm::vec2& value) { shader->SetFloat2(name, value); }
		void SetFloat3(const std::string& name, const glm::vec3& value) { shader->SetFloat3(name, value); }
		void SetFloat4(const std::string& name, const glm::vec4& value) { shader->SetFloat4(name, value); }
		void SetMat4(const std::string& name, const glm::mat4& value) { shader->SetMat4(name, value); }
	};

	struct PhongMaterial : public Material
	{
		Ref<Texture2D> DiffuseTexture = nullptr;
		Ref<Texture2D> SpecularTexture = nullptr;
		Ref<Texture2D> NormalTexture = nullptr;
		Ref<Texture2D> DisplacementTexture = nullptr;
		float HeightScale = 0;
		glm::vec3 Color{ 1.0f };
		float Shininess = 32;
		PhongMaterial() 
		{
			shader = Shader::Create("assets/shaders/Phong.glsl");
		};
	};
}