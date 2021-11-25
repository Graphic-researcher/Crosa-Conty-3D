#include "ccpch.h"
#include "CC3D/Renderer/RenderContext.h"
#include "CC3D/Renderer/Renderer2D.h"


#include "CC3D/Scene/SceneCamera.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/matrix_transform.hpp>

namespace CC3D {

	Scope<RenderContext::SceneData> RenderContext::s_SceneData = CreateScope<RenderContext::SceneData>();

	void RenderContext::Init()
	{
		CC3D_PROFILE_FUNCTION();

		RenderCommand::Init();
		Renderer2D::Init();
	}

	void RenderContext::Shutdown()
	{
		Renderer2D::Shutdown();
	}

	void RenderContext::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void RenderContext::BeginScene(SceneCamera& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetProjection();

	}

	void RenderContext::EndScene()
	{
	}

	void RenderContext::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		// dynamic_cast for shared_ptr that properly respects the reference count control block
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}