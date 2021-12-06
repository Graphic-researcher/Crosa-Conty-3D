#pragma once

#include "CC3D/Renderer/Texture.h"

#include "CC3D/Renderer/Camera.h"
#include "CC3D/Renderer/EditorCamera.h"
#include "CC3D/Renderer/FrameBuffer.h"
#include "CC3D/Renderer/Light.h"

#include "CC3D/Scene/Components/MeshRendererComponent.h"
#include "CC3D/Scene/Components/MaterialComponent.h"
#include "CC3D/Scene/Components/TransformComponent.h"

namespace CC3D {

	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const EditorCamera& camera);
		static void EndScene();

		static void BeginCastShadow(const Ref<Light> directLight, const TransformComponent& lightTranform);
		static void EndCastShadow();

		static void DrawShadow(const glm::mat4& transform, MeshRendererComponent& src);

		// Draw Renderer
		// TODO all renderer extend from Renderer
		static void DrawRenderer(const glm::mat4& transform, MeshRendererComponent& src, MaterialComponent& material, int entityID);

		static Ref<Framebuffer> GetShadowMap() { return m_SceneData->ShadowMap; }

	private:

		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
			glm::mat4 LightSpaceMatrix;
			glm::vec3 ViewPosition;
			
			Ref<Shader> DepthShader;
			Ref<Framebuffer> ShadowMap;
		};

		static SceneData* m_SceneData;
	};
}