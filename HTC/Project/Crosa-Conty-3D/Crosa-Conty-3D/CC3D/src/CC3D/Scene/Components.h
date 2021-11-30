#pragma once

#include "CC3D/Renderer/Camera.h"
#include "CC3D/Scene/SceneCamera.h"
#include "CC3D/Core/UUID.h"
#include "CC3D/Renderer/Texture.h"

//3D Rendering dev:
#include "CC3D/Renderer/Shader.h"
#include "CC3D/Renderer/3Ddev/mesh.h"
#include "CC3D/Renderer/3Ddev/light.h"
#include "CC3D/Renderer/3Ddev/effect.h"
#include "CC3D/Renderer/3Ddev/material.h"

#include<string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace CC3D {
	struct IDComponent
	{
		UUID ID;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
	};

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
	};

	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation) {}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Texture2D> Texture;
		float TilingFactor = 1.0f;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}
	};

	struct CameraComponent
	{
		SceneCamera Camera;

		bool FixedAspectRatio = false;

		/// <summary>
		/// "Primary" decide which camera is main camera that should render to viewport
		/// </summary>
		bool Primary = true; // TODO: think about moving to Scene

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	// Forward declaration
	class ScriptableEntity;

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};

	//3D Rendering Dev:
	struct MaterialComponent
	{
		Ref<Material> MaterialSrc = CreateRef<Material>();
		MaterialType Type = MaterialType::Material_None;
		std::unordered_map<LightType, uint32_t> LightCount =
		{
			{LightType::LightType_None, 0},
			{LightType::LightType_Basic, 0},
			{LightType::LightType_DirLight, 0},
			{LightType::LightType_PointLight, 0},
			{LightType::LightType_SpotLight, 0}
		};
		MaterialComponent() = default;
		MaterialComponent(MaterialType type)
			:Type(type)
		{
			switch (type)
			{
			case MaterialType::Material_None:
				MaterialSrc = CreateRef<Material>();
				return;
			case MaterialType::Material_Emission:
				MaterialSrc = CreateRef<EmissionMaterial>();
				return;
			case MaterialType::Material_Phong:
				MaterialSrc = CreateRef<PhongMaterial>();
				return;
			case MaterialType::Material_Cook_Torrance:
				MaterialSrc = CreateRef<CookTorranceBRDF>();
				return;
			default:
				break;
			}
			CC3D_CORE_ASSERT(false, "Unavailable Material Type");
		}

		void SetMaterialType(MaterialType type)
		{
			MaterialSrc.reset();
			Type = type;
			switch (type)
			{
			case MaterialType::Material_None:
				MaterialSrc = CreateRef<Material>();
				return;
			case MaterialType::Material_Emission:
				MaterialSrc = CreateRef<EmissionMaterial>();
				return;
			case MaterialType::Material_Phong:
				MaterialSrc = CreateRef<PhongMaterial>();
				return;
			case MaterialType::Material_Cook_Torrance:
				MaterialSrc = CreateRef<CookTorranceBRDF>();
				return;
			default:
				break;
			}
			CC3D_CORE_ASSERT(false, "Unavailable Material Type");
		}

		void ResetType()
		{
			MaterialSrc.reset();
			switch (Type)
			{
			case MaterialType::Material_None:
				MaterialSrc = CreateRef<Material>();
				return;
			case MaterialType::Material_Emission:
				MaterialSrc = CreateRef<EmissionMaterial>();
				return;
			case MaterialType::Material_Phong:
				MaterialSrc = CreateRef<PhongMaterial>();
				return;
			case MaterialType::Material_Cook_Torrance:
				MaterialSrc = CreateRef<CookTorranceBRDF>();
				return;
			default:
				break;
			}
			CC3D_CORE_ASSERT(false, "Unavailable Material Type");
		}

		void Bind(const Ref<Cubemap>& envMap = nullptr)
		{
			switch (Type)
			{
			case MaterialType::Material_None:
				MaterialSrc->Bind();
				return;
			case MaterialType::Material_Emission:
				CastRef<EmissionMaterial>(MaterialSrc)->Bind();
				return;
			case MaterialType::Material_Phong:
				CastRef<PhongMaterial>(MaterialSrc)->Bind();
				return;
			case MaterialType::Material_Cook_Torrance:
				CastRef<CookTorranceBRDF>(MaterialSrc)->Bind(envMap);
				return;
			default:
				break;
			}
		}

		void AddLight(LightType type)
		{
			LightCount[type]++;
			switch (Type)
			{
			case MaterialType::Material_None:
				MaterialSrc->Bind();
				MaterialSrc->m_Shader->SetInt(LightTypeMap[type] + "Num", LightCount[type]);
				return;
			case MaterialType::Material_Emission:
				CastRef<EmissionMaterial>(MaterialSrc)->Bind();
				CastRef<EmissionMaterial>(MaterialSrc)->m_Shader->SetInt(LightTypeMap[type] + "Num", LightCount[type]);
				return;
			case MaterialType::Material_Phong:
				CastRef<PhongMaterial>(MaterialSrc)->Bind();
				CastRef<PhongMaterial>(MaterialSrc)->m_Shader->SetInt(LightTypeMap[type] + "Num", LightCount[type]);
				return;
			case MaterialType::Material_Cook_Torrance:
				CastRef<CookTorranceBRDF>(MaterialSrc)->Bind();
				CastRef<CookTorranceBRDF>(MaterialSrc)->m_Shader->SetInt(LightTypeMap[type] + "Num", LightCount[type]);
				return;
			default:
				break;
			}
		}

		void ResetLight()
		{
			for (auto& light : LightCount)
			{
				light.second = 0;
				switch (Type)
				{
				case MaterialType::Material_None:
					MaterialSrc->Bind();
					MaterialSrc->m_Shader->SetInt(LightTypeMap[light.first] + "Num", 0);
					break;
				case MaterialType::Material_Emission:
					CastRef<EmissionMaterial>(MaterialSrc)->Bind();
					CastRef<EmissionMaterial>(MaterialSrc)->m_Shader->SetInt(LightTypeMap[light.first] + "Num", 0);
					break;
				case MaterialType::Material_Phong:
					CastRef<PhongMaterial>(MaterialSrc)->Bind();
					CastRef<PhongMaterial>(MaterialSrc)->m_Shader->SetInt(LightTypeMap[light.first] + "Num", 0);
					break;
				case MaterialType::Material_Cook_Torrance:
					CastRef<CookTorranceBRDF>(MaterialSrc)->Bind();
					CastRef<CookTorranceBRDF>(MaterialSrc)->m_Shader->SetInt(LightTypeMap[light.first] + "Num", 0);
					break;
				default:
					break;
				}
			}
		}

		void Set(EditorCamera& camera, const glm::mat4& transform,int entity)
		{
			switch (Type)
			{
			case MaterialType::Material_None:
				MaterialSrc->m_Shader->SetMat4("u_ViewProjection", camera.GetViewProjection());
				MaterialSrc->m_Shader->SetMat4("u_Transform", transform);
				MaterialSrc->m_Shader->SetInt("a_EntityID", entity);

				return;
			case MaterialType::Material_Emission:
			{
				CastRef<EmissionMaterial>(MaterialSrc)->m_Shader->SetMat4("u_ViewProjection", camera.GetViewProjection());
				CastRef<EmissionMaterial>(MaterialSrc)->m_Shader->SetMat4("u_Transform", transform);
				CastRef<EmissionMaterial>(MaterialSrc)->m_Shader->SetFloat3("u_ViewPos", camera.GetPosition());
				CastRef<EmissionMaterial>(MaterialSrc)->m_Shader->SetInt("a_EntityID", entity);
				return;
			}
			case MaterialType::Material_Phong:
				CastRef<PhongMaterial>(MaterialSrc)->m_Shader->SetMat4("u_ViewProjection", camera.GetViewProjection());
				CastRef<PhongMaterial>(MaterialSrc)->m_Shader->SetMat4("u_Transform", transform);
				CastRef<PhongMaterial>(MaterialSrc)->m_Shader->SetFloat3("u_ViewPos", camera.GetPosition());
				CastRef<PhongMaterial>(MaterialSrc)->m_Shader->SetInt("a_EntityID", entity);
				return;
			case MaterialType::Material_Cook_Torrance:
				CastRef<CookTorranceBRDF>(MaterialSrc)->m_Shader->SetMat4("u_ViewProjection", camera.GetViewProjection());
				CastRef<CookTorranceBRDF>(MaterialSrc)->m_Shader->SetMat4("u_Transform", transform);
				CastRef<CookTorranceBRDF>(MaterialSrc)->m_Shader->SetFloat3("u_ViewPos", camera.GetPosition());
				CastRef<CookTorranceBRDF>(MaterialSrc)->m_Shader->SetInt("a_EntityID", entity);
				return;
			default:
				break;
			}
		}

		Ref<Shader>& GetShader()
		{
			switch (Type)
			{
			case MaterialType::Material_None:
				return MaterialSrc->m_Shader;
			case MaterialType::Material_Emission:
				return CastRef<EmissionMaterial>(MaterialSrc)->m_Shader;
			case MaterialType::Material_Phong:
				return CastRef<PhongMaterial>(MaterialSrc)->m_Shader;
			case MaterialType::Material_Cook_Torrance:
				return CastRef<CookTorranceBRDF>(MaterialSrc)->m_Shader;
			default:
				return MaterialSrc->m_Shader;
				break;
			}
		}
	};

	struct MeshComponent
	{
		Ref<TriMesh> Mesh = CreateRef<TriMesh>();
		MeshComponent():Mesh(TriMesh::Create(MeshType::Sphere, 1)) {}
		MeshComponent(const MeshComponent&) = default;
		MeshComponent(const Ref<TriMesh>& mesh)
			: Mesh(mesh) {}
		MeshComponent(const std::string& path)
			:Mesh(TriMesh::Create(path)) {}
		MeshComponent(const MeshType& type, const uint32_t& sample = 1)
			:Mesh(TriMesh::Create(type, sample)) {}

		void Reset() { Mesh.reset(); Mesh = TriMesh::Create(); }
		void Reload(const std::string& path)
		{
			Mesh.reset();
			Mesh = TriMesh::Create(path);
		}
		void Reload(const MeshType& type, const uint32_t& sample = 1)
		{
			Mesh.reset();
			Mesh = TriMesh::Create(type, sample);
		}
	};

	struct LightComponent
	{
		Ref<Light> LightSrc = CreateRef<Light>();
		LightType Type = LightType::LightType_Basic;

		LightComponent() = default;

		void SetLight(LightType type)
		{
			LightSrc.reset();
			switch (type)
			{
			case LightType::LightType_None:
				LightSrc = nullptr;
				return;
			case LightType::LightType_Basic:
				LightSrc = CreateRef<Light>();
				return;
			case LightType::LightType_DirLight:
				LightSrc = CreateRef<DirLight>();
				return;
			case LightType::LightType_PointLight:
				LightSrc = CreateRef<PointLight>();
				return;
			case LightType::LightType_SpotLight:
				LightSrc = CreateRef<SpotLight>();
				return;
			default:
				break;
			}
			CC3D_ASSERT(false, "Unavailable light type!");
		}

		void ResetType()
		{
			LightSrc.reset();
			switch (Type)
			{
			case LightType::LightType_None:
				LightSrc = nullptr;
				return;
			case LightType::LightType_Basic:
				LightSrc = CreateRef<Light>();
				return;
			case LightType::LightType_DirLight:
				LightSrc = CreateRef<DirLight>();
				return;
			case LightType::LightType_PointLight:
				LightSrc = CreateRef<PointLight>();
				return;
			case LightType::LightType_SpotLight:
				LightSrc = CreateRef<SpotLight>();
				return;
			default:
				break;
			}
			CC3D_ASSERT(false, "Unavailable light type!");
		}

		void SetType(LightType type)
		{
			LightSrc.reset();
			Type = type;
			switch (type)
			{
			case LightType::LightType_None:
				LightSrc = nullptr;
				return;
			case LightType::LightType_Basic:
				LightSrc = CreateRef<Light>();
				return;
			case LightType::LightType_DirLight:
				LightSrc = CreateRef<DirLight>();
				return;
			case LightType::LightType_PointLight:
				LightSrc = CreateRef<PointLight>();
				return;
			case LightType::LightType_SpotLight:
				LightSrc = CreateRef<SpotLight>();
				return;
			default:
				break;
			}
			CC3D_ASSERT(false, "Unavailable light type!");
		}

		void Bind(const Ref<Shader>& shader, const glm::vec3& position, const uint32_t& slot = 0)
		{
			switch (Type)
			{
			case LightType::LightType_None:
				return;
			case LightType::LightType_Basic:
				LightSrc->Bind(shader, position, slot);
				return;
			case LightType::LightType_DirLight:
				CastRef<DirLight>(LightSrc)->Bind(shader, position, slot);
				return;
			case LightType::LightType_PointLight:
				CastRef<PointLight>(LightSrc)->Bind(shader, position, slot);
				return;
			case LightType::LightType_SpotLight:
				CastRef<SpotLight>(LightSrc)->Bind(shader, position, slot);
				return;
			default:
				break;
			}
			CC3D_ASSERT(false, "Unavailable light type!");
		}
	};


	// Physics

	struct Rigidbody2DComponent
	{
		enum class BodyType { Static = 0, Dynamic, Kinematic };
		BodyType Type = BodyType::Static;
		bool FixedRotation = false;

		// Storage for runtime
		void* RuntimeBody = nullptr;

		Rigidbody2DComponent() = default;
		Rigidbody2DComponent(const Rigidbody2DComponent&) = default;
	};

	struct BoxCollider2DComponent
	{
		glm::vec2 Offset = { 0.0f, 0.0f };
		glm::vec2 Size = { 0.5f, 0.5f };

		// TODO(Yan): move into physics material in the future maybe
		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;

		// Storage for runtime
		void* RuntimeFixture = nullptr;

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
	};
}