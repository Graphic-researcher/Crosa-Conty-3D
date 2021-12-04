#include "ccpch.h"
#include "BatchRenderer.h"

#include "CC3D/Renderer/VertexArray.h"
#include "CC3D/Renderer/Shader.h"
#include "CC3D/Renderer/RenderCommand.h"
namespace CC3D
{
	namespace Utils
	{
		void ConnectIndices(std::vector<uint32_t>& former, std::vector<uint32_t>& later)
		{
			// 不能直接加在后面，因为这样会导致indexbuffer出错，新的模型的indexbuffer是重新开始的
			if (former.size() > 0)
			{
				uint32_t offset = *std::max_element(former.begin(), former.end());
				for (uint32_t& index : later)
				{
					index += offset + 1;
				}

				former.insert(former.end(), later.begin(), later.end());
			}
			else
				former.insert(former.end(), later.begin(), later.end());
		}
	}
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
		static const uint32_t MaxTris = 20000;
		static const uint32_t MaxVertices = MaxTris * 3;
		static const uint32_t MaxIndices = MaxTris * 3;
		static const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps

		Ref<VertexArray> TriVertexArray;
		Ref<VertexBuffer> TriVertexBuffer;
		// TODO material
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		std::vector<uint32_t> TriIndices;

		uint32_t TriIndexCount = 0;
		TriVertex* TriVertexBufferBase = nullptr;
		TriVertex* TriVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;// TODO 添加超过32个纹理的警告提示
		uint32_t TextureSlotIndex = 1; // 0 = white texture

		BatchRenderer::Statistics Stats;
	};

	static RendererData s_Data;

	void BatchRenderer::Init()
	{
		CC3D_PROFILE_FUNCTION();

		s_Data.TriVertexArray = VertexArray::Create();


		s_Data.TriVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(TriVertex));
		s_Data.TriVertexBuffer->SetLayout({
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
		s_Data.TriVertexArray->AddVertexBuffer(s_Data.TriVertexBuffer);
		s_Data.TriVertexBufferBase = new TriVertex[s_Data.MaxVertices];

		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffff00ff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		// TODO 3d Model default shader
		s_Data.TextureShader = Shader::Create("assets/shaders/BatchPhong.glsl");
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

		// Set first texture slot to 0
		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

	}
	void BatchRenderer::Shutdown()
	{
		CC3D_PROFILE_FUNCTION();

		delete[] s_Data.TriVertexBufferBase;
	}

	void BatchRenderer::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		CC3D_PROFILE_FUNCTION();

		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", viewProj);
		StartBatch();
	}

	void BatchRenderer::BeginScene(const EditorCamera& camera)
	{
		CC3D_PROFILE_FUNCTION();

		glm::mat4 viewProj = camera.GetViewProjection();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", viewProj);

		StartBatch();
	}

	void BatchRenderer::EndScene()
	{
		CC3D_PROFILE_FUNCTION();
		Flush();
	}

	void BatchRenderer::Flush()
	{
		if (s_Data.TriIndexCount == 0)
			return; // Nothing to draw
		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.TriVertexBufferPtr - (uint8_t*)s_Data.TriVertexBufferBase);
		s_Data.TriVertexBuffer->SetData(s_Data.TriVertexBufferBase, dataSize);

		// Bind textures
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)// why here
			s_Data.TextureSlots[i]->Bind(i);

		// Create index buffer
		Ref<IndexBuffer> TriIB = IndexBuffer::Create(s_Data.TriIndices.data(), s_Data.TriIndices.size());
		s_Data.TriVertexArray->SetIndexBuffer(TriIB);
		//std::vector<uint32_t> in = s_Data.TriIndices;
		s_Data.TriIndices.clear();


		s_Data.TriVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.TriVertexArray, s_Data.TriIndexCount);
		s_Data.Stats.DrawCalls++;
		s_Data.TextureShader->Unbind();
	}

	void BatchRenderer::DrawMesh(const glm::mat4& transform, MeshRendererComponent& src, int entityID)
	{
		//TriVertex vertex = MeshRendererComponent src
	}

	void BatchRenderer::DrawMesh(const glm::mat4& transform, MeshRendererComponent& src, MaterialComponent& material, int entityID)
	{
		//TODO default Material
		CC3D_PROFILE_FUNCTION();

		const float textureIndex = 0.0f; // White Texture
		const float tilingFactor = 1.0f;
		std::vector<Mesh> meshes = src.m_Meshes;
		for (size_t i = 0; i < meshes.size(); i++)
		{
			if (s_Data.TriIndexCount >= RendererData::MaxIndices)
				NextBatch();

			// 不能直接加在后面，因为这样会导致indexbuffer出错，新的模型的indexbuffer是重新开始的
			Utils::ConnectIndices(s_Data.TriIndices, meshes[i].indices);

			const size_t TriVertexCount = meshes[i].vertices.size();
			for (size_t j = 0; j < TriVertexCount; j++)
			{
				// TODO multiple Textures
				s_Data.TriVertexBufferPtr->Position = glm::vec3(transform * glm::vec4(meshes[i].vertices[j].Position, 1));
				s_Data.TriVertexBufferPtr->Normal = meshes[i].vertices[j].Normal;//把世界坐标的法向量传进去
				s_Data.TriVertexBufferPtr->Tangent = meshes[i].vertices[j].Tangent;
				s_Data.TriVertexBufferPtr->Bitangent = meshes[i].vertices[j].Bitangent;

				s_Data.TriVertexBufferPtr->Color = material.Color;
				s_Data.TriVertexBufferPtr->TexCoord = meshes[i].vertices[j].TexCoords;
				s_Data.TriVertexBufferPtr->TexIndex = textureIndex;
				s_Data.TriVertexBufferPtr->TilingFactor = tilingFactor;
				s_Data.TriVertexBufferPtr->EntityID = entityID;
				s_Data.TriVertexBufferPtr++;
			}

			// TODO 计数
			s_Data.TriIndexCount += meshes[i].indices.size();

			s_Data.Stats.TriangleCount += meshes[i].indices.size() / 3;

		}
	}

	void BatchRenderer::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}

	BatchRenderer::Statistics BatchRenderer::GetStats()
	{
		return s_Data.Stats;
	}

	void BatchRenderer::StartBatch()
	{
		s_Data.TriIndexCount = 0;
		s_Data.TriVertexBufferPtr = s_Data.TriVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void BatchRenderer::NextBatch()
	{
		Flush();
		StartBatch();
	}

}
