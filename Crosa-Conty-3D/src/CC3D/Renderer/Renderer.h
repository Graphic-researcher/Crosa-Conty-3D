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
		static void Flush();// ÖØÐÂ¿ªÆô

		// Primitives
		static void DrawMesh(const glm::mat4& transform, MeshRendererComponent& src, int entityID);

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
		static void StartBatch();
		static void NextBatch();
	};
}