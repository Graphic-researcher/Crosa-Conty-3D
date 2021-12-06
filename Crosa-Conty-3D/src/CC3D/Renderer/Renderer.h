#pragma once

#include "CC3D/Renderer/Texture.h"

#include "CC3D/Renderer/Camera.h"
#include "CC3D/Renderer/EditorCamera.h"

#include "CC3D/Scene/Components/MeshRendererComponent.h"
#include "CC3D/Scene/Components/MaterialComponent.h"

namespace CC3D {

	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const EditorCamera& camera);
		static void EndScene();

		// Draw Renderer
		// TODO all renderer extend from Renderer
		static void DrawRenderer(const glm::mat4& transform, MeshRendererComponent& src, MaterialComponent& material, int entityID);

	private:

		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
			glm::vec3 ViewPosition;
		};

		static SceneData* m_SceneData;
	};
}