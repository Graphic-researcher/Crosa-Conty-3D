//#pragma once
//
//#include "CC3D/Renderer/RenderCommand.h"
//#include "CC3D/Scene/SceneCamera.h"
//#include "CC3D/Renderer/Shader.h"
//
//namespace CC3D {
//	class Renderer
//	{
//	public:
//		static void Init();
//		static void Shutdown();
//
//		static void OnWindowResize(uint32_t width, uint32_t height);
//
//		static void BeginScene(SceneCamera& camera);
//		static void EndScene();
//
//		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
//
//		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
//
//	private:
//		struct SceneData
//		{
//			glm::mat4 ViewProjectionMatrix;
//		};
//
//		static Scope<Renderer::SceneData> s_SceneData;
//	};
//
//}