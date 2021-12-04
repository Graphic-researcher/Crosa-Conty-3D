#pragma once

#include "CC3D/Renderer/Texture.h"

#include "CC3D/Renderer/Camera.h"
#include "CC3D/Renderer/EditorCamera.h"

#include "CC3D/Scene/Components/MeshRendererComponent.h"
#include "CC3D/Scene/Components/MaterialComponent.h"

namespace CC3D {

	class BatchRenderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const EditorCamera& camera);
		static void EndScene();
		static void Flush();// ÖØÐÂ¿ªÆô

		// Primitives
		// For Batch rendering
		static void DrawMesh(const glm::mat4& transform, MeshRendererComponent& src, MaterialComponent& material, int entityID);
		static void DrawMesh(const glm::mat4& transform, MeshRendererComponent& src, int entityID);

		// Draw Renderer
		// TODO all renderer extend from Renderer
		static void DrawRenderer(const glm::mat4& transform, MeshRendererComponent& src, MaterialComponent& material, int entityID);

		// Stats
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t TriangleCount = 0;

			uint32_t GetTotalVertexCount() const { return TriangleCount * 3; }
			uint32_t GetTotalIndexCount() const { return TriangleCount * 3; }
		};
		static void ResetStats();
		static Statistics GetStats();
	private:

		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
		static void StartBatch();
		static void NextBatch();
	};
}