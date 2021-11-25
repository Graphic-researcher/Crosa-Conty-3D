#pragma once

#include "CC3D/Scene/SceneCamera.h"
#include "CC3D/Renderer/RenderCommand.h"
#include "CC3D/Renderer/Shader.h"

namespace CC3D {
	class RenderContext
	{
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(SceneCamera& camera);
		static void EndScene();

		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static Scope<RenderContext::SceneData> s_SceneData;
	};

}