#include "ccpch.h"
#include "Renderer.h"

#include "CC3D/Renderer/VertexArray.h"
#include "CC3D/Renderer/Shader.h"
#include "CC3D/Renderer/RenderCommand.h"
//
//#include <glm/gtc/matrix_transform.hpp>
//
//namespace CC3D {
//
//	Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();
//
//	void Renderer::Init()
//	{
//		CC3D_PROFILE_FUNCTION();
//
//		RenderCommand::Init();
//		Renderer2D::Init();
//	}
//
//	void Renderer::Shutdown()
//	{
//		Renderer2D::Shutdown();
//	}
//
//	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
//	{
//		RenderCommand::SetViewport(0, 0, width, height);
//	}
//
//	void Renderer::BeginScene(SceneCamera& camera)
//	{
//		s_SceneData->ViewProjectionMatrix = camera.GetProjection();
//
//	}
//
//	void Renderer::EndScene()
//	{
//	}
//
//	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
//	{
//		shader->Bind();
//		// dynamic_cast for shared_ptr that properly respects the reference count control block
//		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
//		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);
//
//		vertexArray->Bind();
//		RenderCommand::DrawIndexed(vertexArray);
//	}
//}
namespace CC3D
{
	struct TriVertex;

	struct RendererData
	{
		static const uint32_t MaxTris = 20000;
		static const uint32_t MaxVertices = MaxTris * 3;
		static const uint32_t MaxIndices = MaxTris * 3;
		static const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps

		Ref<VertexArray> TriVertexArray;
		Ref<VertexBuffer> TriVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t TriIndexCount = 0;
		TriVertex* TriVertexBufferBase = nullptr;
		TriVertex* TriVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;// TODO 添加超过32个纹理的警告提示
		uint32_t TextureSlotIndex = 1; // 0 = white texture

		glm::vec4 TriVertexPositions[4];
		Renderer::Statistics Stats;
	};

	static RendererData s_Data;

	void Renderer::Init()
	{
		
	}
	void Renderer::Shutdown()
	{

	}



	void Renderer::Flush()
	{
		if (s_Data.TriIndexCount == 0)
			return; // Nothing to draw
		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.TriVertexBufferPtr - (uint8_t*)s_Data.TriVertexBufferBase);
		s_Data.TriVertexBuffer->SetData(s_Data.TriVertexBufferBase, dataSize);

		// Bind textures
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)// why here
			s_Data.TextureSlots[i]->Bind(i);
		RenderCommand::DrawIndexed(s_Data.TriVertexArray, s_Data.TriIndexCount);
		s_Data.Stats.DrawCalls++;
	}

	void Renderer::DrawMesh(const glm::mat4& transform, MeshRendererComponent& src, int entityID)
	{
		//TriVertex vertex = MeshRendererComponent src
	}

	void Renderer::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}

	Renderer::Statistics Renderer::GetStats()
	{
		return s_Data.Stats;
	}

	void Renderer::StartBatch()
	{
		s_Data.TriIndexCount = 0;
		s_Data.TriVertexBufferPtr = s_Data.TriVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer::NextBatch()
	{
		Flush();
		StartBatch();
	}

}
