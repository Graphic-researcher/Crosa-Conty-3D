#include "ccpch.h"
#include "CC3D/Renderer/Renderer.h"

#include "CC3D/Renderer/VertexArray.h"
#include "CC3D/Renderer/Shader.h"
#include "CC3D/Renderer/RenderCommand.h"
#include "CC3D/Math/Math.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/matrix.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <imgui.h>
namespace CC3D
{
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;
	// TODO 区分普通渲染和batch rendering
	struct MeshVertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec3 Tangent;
		glm::vec3 Bitangent;
		glm::vec2 TexCoord;

		// Editor-only
		int EntityID;
	};

	void Renderer::Init()
	{
		CC3D_PROFILE_FUNCTION();

		// TODO ShadowMap 选项调节
		FramebufferSpecification fbSpec;
		fbSpec.Attachments = {FramebufferTextureFormat::Depth };
		fbSpec.Width = 1024;
		fbSpec.Height = 1024;
		m_SceneData->ShadowMap = Framebuffer::Create(fbSpec);
		m_SceneData->DepthShader = Shader::Create("assets/shaders/ShadowMap.glsl");
	}
	void Renderer::Shutdown()
	{
		CC3D_PROFILE_FUNCTION();
	}

	void Renderer::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		CC3D_PROFILE_FUNCTION();
		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);
		m_SceneData->ViewProjectionMatrix = camera.GetProjection() * glm::inverse(transform);
		Math::GetTranslation(transform, m_SceneData->ViewPosition);
	}

	void Renderer::BeginScene(const EditorCamera& camera)
	{
		CC3D_PROFILE_FUNCTION();
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjection();
		m_SceneData->ViewPosition = camera.GetPosition();
	}

	void Renderer::EndScene()
	{
		CC3D_PROFILE_FUNCTION();
	}

	bool Renderer::BeginCastShadow(const Ref<Light> directLight, const TransformComponent& lightTranform)
	{
		if (!directLight || directLight->Intensity == 0)
			return false;

		m_SceneData->ShadowMap->Bind();
		RenderCommand::SetClearColor(glm::vec4{ 0.0f, 0.0f, 0.0f, 1 });
		RenderCommand::Clear();
		// Clear our entity ID attachment to -1

		m_SceneData->DepthShader->Bind();

		// TODO move to DirectLight struction function
		glm::vec3 lightDirection = glm::toMat4(glm::quat(lightTranform.GlobalRotation)) * glm::vec4(0, 0, 1, 0);
		glm::vec3 lightpos = 10.0f * lightDirection;
		glm::vec3 lightTarget = -10.0f * lightDirection;
		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 15.0f);
		
		glm::mat4 lightView = glm::lookAt(lightpos, lightTarget, glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;

		m_SceneData->LightSpaceMatrix = lightSpaceMatrix;
		m_SceneData->LightPosition = lightpos;
		m_SceneData->DepthShader->SetMat4("u_LightSpaceMatrix", lightSpaceMatrix);

		return true;
	}

	void Renderer::EndCastShadow()
	{
		m_SceneData->DepthShader->Unbind();
		m_SceneData->ShadowMap->Unbind();
	}

	void Renderer::DrawShadow(const glm::mat4& transform, MeshRendererComponent& src)
	{
		m_SceneData->DepthShader->SetMat4("u_Transform", transform);

		std::vector<Mesh>& meshes = src.m_Meshes;
		std::vector<uint32_t> indecies;
		Ref<VertexArray> vertexArray = VertexArray::Create();
		size_t size = meshes.size();
		for (size_t i = 0; i < size; i++)
		{
			const size_t TriVertexCount = meshes[i].vertices.size();

			indecies = meshes[i].indices;

			Ref<VertexBuffer> m_VertexBuffers = VertexBuffer::Create(TriVertexCount * sizeof(MeshVertex));
			m_VertexBuffers->SetLayout({
				{ ShaderDataType::Float3, "a_Position"		},
				});
			glm::vec3* MeshVertexBuffer = new glm::vec3[TriVertexCount];
			for (size_t j = 0; j < TriVertexCount; j++)
			{
				MeshVertexBuffer[j] = meshes[i].vertices[j].Position;
			}

			m_VertexBuffers->SetData(MeshVertexBuffer, TriVertexCount * sizeof(glm::vec3));
			delete[] MeshVertexBuffer;
			vertexArray->AddVertexBuffer(m_VertexBuffers);
		}

		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indecies.data(), indecies.size());
		vertexArray->SetIndexBuffer(indexBuffer);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray, indecies.size());
	}

	void Renderer::DrawRenderer(const glm::mat4& transform, MeshRendererComponent& src, MaterialComponent& material, int entityID)
	{

		material.material->SetMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		material.material->SetMat4("u_LightSpaceMatrix", m_SceneData->LightSpaceMatrix);
		material.material->SetMat4("u_Transform", transform);

		material.material->SetFloat3("u_ViewPos", m_SceneData->ViewPosition);
		material.material->SetFloat3("u_LightPos", m_SceneData->LightPosition);
		

		material.material->SetInt("ShadowMap", m_SceneData->ShadowMap->GetDepthAttachmentRendererID());

		std::vector<Mesh>& meshes = src.m_Meshes;
		std::vector<uint32_t> indecies;
		Ref<VertexArray> vertexArray = VertexArray::Create();
		size_t size = meshes.size();
		for (size_t i = 0; i < size; i++)
		{
			const size_t TriVertexCount = meshes[i].vertices.size();

			indecies = meshes[i].indices;

			Ref<VertexBuffer> m_VertexBuffers = VertexBuffer::Create(TriVertexCount * sizeof(MeshVertex));
			m_VertexBuffers->SetLayout({
				{ ShaderDataType::Float3, "a_Position"		},
				{ ShaderDataType::Float3, "a_Normal"		},
				{ ShaderDataType::Float3, "a_Tangent"		},
				{ ShaderDataType::Float3, "a_Bitangent"		},
				{ ShaderDataType::Float2, "a_TexCoord"		},

				{ ShaderDataType::Int,    "a_EntityID"		}
				});
			MeshVertex* MeshVertexBuffer = new MeshVertex[TriVertexCount];
			for (size_t j = 0; j < TriVertexCount; j++)
			{
				MeshVertexBuffer[j].Position = meshes[i].vertices[j].Position;
				MeshVertexBuffer[j].Normal = meshes[i].vertices[j].Normal;//把世界坐标的法向量传进去
				MeshVertexBuffer[j].Tangent = meshes[i].vertices[j].Tangent;
				MeshVertexBuffer[j].Bitangent = meshes[i].vertices[j].Bitangent;
				MeshVertexBuffer[j].TexCoord = meshes[i].vertices[j].TexCoords;

				MeshVertexBuffer[j].EntityID = entityID;
			}
			
			m_VertexBuffers->SetData(MeshVertexBuffer, TriVertexCount * sizeof(MeshVertex));
			delete[] MeshVertexBuffer;
			vertexArray->AddVertexBuffer(m_VertexBuffers);
		}

		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indecies.data(), indecies.size());
		vertexArray->SetIndexBuffer(indexBuffer);

		
		
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray, indecies.size());


		//vertexArray.reset();
	}


}
