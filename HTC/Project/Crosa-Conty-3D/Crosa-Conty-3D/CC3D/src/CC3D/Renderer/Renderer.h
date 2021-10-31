#pragma once

#include "CC3D/Renderer/RenderCommand.h"
#include "CC3D/Renderer/OrthographicCamera.h"
#include "CC3D/Renderer/OrthographicCamera.h"
#include "CC3D/Renderer/Shader.h"

namespace CC3D {

	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();
				/// Submit material			   and		 		  geometry information							   transform matrix
		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
		static void OnWindowResize(uint32_t width, uint32_t height);

		
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static Scope<SceneData> s_SceneData;
	};


}
