#include "ccpch.h"
#include "Renderer.h"

#include "CC3D/Renderer/VertexArray.h"
#include "CC3D/Renderer/Shader.h"
#include "CC3D/Renderer/RenderCommand.h"
namespace CC3D
{
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;
	// TODO 区分普通渲染和batch rendering
	struct TriVertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec3 Tangent;
		glm::vec3 Bitangent;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;

		// Editor-only
		int EntityID;
	};

	struct RendererData
	{
		Ref<VertexArray> TriVertexArray;
	};

	static RendererData s_Data;

	void Renderer::Init()
	{
		CC3D_PROFILE_FUNCTION();
		s_Data.TriVertexArray = VertexArray::Create();
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
	}

	void Renderer::BeginScene(const EditorCamera& camera)
	{
		CC3D_PROFILE_FUNCTION();
		glm::mat4 viewProj = camera.GetViewProjection();
		m_SceneData->ViewProjectionMatrix = viewProj;
	}

	void Renderer::EndScene()
	{
		CC3D_PROFILE_FUNCTION();
	}

	void Renderer::DrawRenderer(const glm::mat4& transform, MeshRendererComponent& src, MaterialComponent& material, int entityID)
	{
		material.material->shader->SetMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		const float textureIndex = 1.0f; // White Texture
		const float tilingFactor = 1.0f;
		std::vector<Mesh>& meshes = src.m_Meshes;
		std::vector<uint32_t> indecies;
		Ref<VertexArray> vertexArray = VertexArray::Create();
		size_t size = meshes.size();
		for (size_t i = 0; i < size; i++)
		{
			const size_t TriVertexCount = meshes[i].vertices.size();

			indecies = meshes[i].indices;

			Ref<VertexBuffer> m_VertexBuffers = VertexBuffer::Create(TriVertexCount * sizeof(TriVertex));
			m_VertexBuffers->SetLayout({
				{ ShaderDataType::Float3, "a_Position"		},
				{ ShaderDataType::Float3, "a_Normal"		},
				{ ShaderDataType::Float3, "a_Tangent"		},
				{ ShaderDataType::Float3, "a_Bitangent"		},

				{ ShaderDataType::Float4, "a_Color"			},
				{ ShaderDataType::Float2, "a_TexCoord"		},
				{ ShaderDataType::Float,  "a_TexIndex"		},
				{ ShaderDataType::Float,  "a_TilingFactor"	},
				{ ShaderDataType::Int,    "a_EntityID"		}
				});
			TriVertex* TriVertexBuffer = new TriVertex[TriVertexCount];
			for (size_t j = 0; j < TriVertexCount; j++)
			{
				TriVertexBuffer[j].Position = glm::vec3(transform * glm::vec4(meshes[i].vertices[j].Position, 1));
				TriVertexBuffer[j].Normal = meshes[i].vertices[j].Normal;//把世界坐标的法向量传进去
				TriVertexBuffer[j].Tangent = meshes[i].vertices[j].Tangent;
				TriVertexBuffer[j].Bitangent = meshes[i].vertices[j].Bitangent;

				TriVertexBuffer[j].Color = material.Color;
				TriVertexBuffer[j].TexCoord = meshes[i].vertices[j].TexCoords;
				TriVertexBuffer[j].TexIndex = textureIndex;
				TriVertexBuffer[j].TilingFactor = tilingFactor;
				TriVertexBuffer[j].EntityID = entityID;
			}
			
			m_VertexBuffers->SetData(TriVertexBuffer, TriVertexCount * sizeof(TriVertex));
			delete[] TriVertexBuffer;
			vertexArray->AddVertexBuffer(m_VertexBuffers);
		}

		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indecies.data(), indecies.size());
		vertexArray->SetIndexBuffer(indexBuffer);

		
		
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray, indecies.size());


		//vertexArray.reset();
	}


}
