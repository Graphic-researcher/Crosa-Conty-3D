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
		int slot = 0;

		virtual void Bind() { shader->Bind(); }
		virtual void Unbind() { shader->Unbind(); }
		void SetInt(const std::string& name, int value) { shader->SetInt(name, value); }
		void SetIntArray(const std::string& name, int* values, uint32_t count) { shader->SetIntArray(name, values, count); }
		void SetFloat(const std::string& name, float value) { shader->SetFloat(name, value); }
		void SetFloat2(const std::string& name, const glm::vec2& value) { shader->SetFloat2(name, value); }
		void SetFloat3(const std::string& name, const glm::vec3& value) { shader->SetFloat3(name, value); }
		void SetFloat4(const std::string& name, const glm::vec4& value) { shader->SetFloat4(name, value); }
		void SetMat4(const std::string& name, const glm::mat4& value) { shader->SetMat4(name, value); }
		void SetTexture(const std::string& name, Ref<Texture2D>& texture){
			if (texture)
			{
				texture->Bind(slot);
				shader->SetInt(name, slot);
				slot++;
			}
			else
			{
				CC3D_CORE_WARN(shader->GetName()+" Shader : Texture is unavailable");
				texture = Texture2D::Create(1, 1);
				uint32_t data = 0xffffffff;// Ã»ÓÐÎÆÀí
				texture->SetData(&data, sizeof(uint32_t));
			}
		}
		void ResetTexture(Ref<Texture2D>& texture, const std::string path = std::string())
		{
			if (path.empty())
			{
				texture = Texture2D::Create(1, 1);
				uint32_t data = 0xffffffff;
				texture->SetData(&data, sizeof(uint32_t));
			}
			else
			{
				texture.reset();
				texture = Texture2D::Create(path);
			}
		}
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
			ResetTexture(DiffuseTexture);
			ResetTexture(SpecularTexture);
			ResetTexture(NormalTexture);
			ResetTexture(DisplacementTexture);
		};
		virtual void Bind() override
		{
			shader->Bind();
			SetTexture("u_Material.diffuse", DiffuseTexture);
			SetTexture("u_Material.specular", SpecularTexture);
			if (NormalTexture->IsLoaded())
				shader->SetInt("u_UseNormalMap", 1);
			else
				shader->SetInt("u_UseNormalMap", 0);
			SetTexture("u_Material.normalMap", NormalTexture);
			SetTexture("u_Material.displacementMap", DisplacementTexture);
			shader->SetFloat3("u_Material.color", Color);
			shader->SetFloat("u_HeightScale", HeightScale);
			shader->SetFloat("u_Material.shininess", Shininess);
			// TODO
			slot = 0;
		}
		virtual void Unbind() override
		{
			shader->Unbind();
		}
	};
}