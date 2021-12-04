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

		virtual void Bind() = 0;
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
		virtual void Bind()
		{
			shader->Bind();
		}
	};
}