#include "ccpch.h"
#include "Renderer.h"

#include "CC3D/Renderer/VertexArray.h"
#include "CC3D/Renderer/Shader.h"
#include "CC3D/Renderer/RenderCommand.h"
namespace CC3D
{
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

		uint32_t TriIndexCount = 0;
		TriVertex* TriVertexBufferBase = nullptr;
		TriVertex* TriVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;// TODO 添加超过32个纹理的警告提示
		uint32_t TextureSlotIndex = 1; // 0 = white texture

		Renderer::Statistics Stats;
	};

	static RendererData s_Data;

	void Renderer::Init()
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


		uint32_t* TriIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			TriIndices[i + 0] = offset + 0;
			TriIndices[i + 1] = offset + 1;
			TriIndices[i + 2] = offset + 2;

			TriIndices[i + 3] = offset + 2;
			TriIndices[i + 4] = offset + 3;
			TriIndices[i + 5] = offset + 0;

			offset += 4;// +4 是因为本质上是画4个点
		}

		Ref<IndexBuffer> TriIB = IndexBuffer::Create(TriIndices, s_Data.MaxIndices);
		s_Data.TriVertexArray->SetIndexBuffer(TriIB);
		delete[] TriIndices;

		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffff00ff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		// TODO 3d Model default shader
		s_Data.TextureShader = Shader::Create("assets/shaders/Phong.glsl");
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

		// Set first texture slot to 0
		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

	}
	void Renderer::Shutdown()
	{
		CC3D_PROFILE_FUNCTION();

		delete[] s_Data.TriVertexBufferBase;
	}

	void Renderer::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		CC3D_PROFILE_FUNCTION();

		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", viewProj);

		StartBatch();
	}

	void Renderer::BeginScene(const EditorCamera& camera)
	{
		CC3D_PROFILE_FUNCTION();

		glm::mat4 viewProj = camera.GetViewProjection();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", viewProj);

		StartBatch();
	}

	void Renderer::EndScene()
	{
		CC3D_PROFILE_FUNCTION();
		Flush();
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

	void Renderer::DrawRenderer(const glm::mat4& transform, MeshRendererComponent& src, MaterialComponent& material, int entityID)
	{
		
		/*const std::vector<Mesh>& meshes = src.m_Meshes;
		std::vector<uint32_t> indecies;

		Ref<VertexArray> vertexArray = VertexArray::Create();

		size_t size = meshes.size();
		for (size_t i = 0; i < size; i++)
		{
			indecies.insert(indecies.end(),meshes[i].indices.begin(), meshes[i].indices.end());
			Ref<VertexBuffer> m_VertexBuffers = VertexBuffer::Create(meshes[i].vertices.size() * sizeof(Vertex));
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
			m_VertexBuffers->SetData(meshes[i].vertices.data(), meshes[i].vertices.size() * sizeof(Vertex));
			vertexArray->AddVertexBuffer(m_VertexBuffers);
		}

		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indecies.data(), indecies.size());
		vertexArray->SetIndexBuffer(indexBuffer);

		Ref<Shader> shader = Shader::Create("assets/shaders/Phong.glsl");

		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", viewProj);

		RenderCommand::DrawIndexed(vertexArray, indecies.size());*/
	}

	void Renderer::DrawMesh(const glm::mat4& transform, MeshRendererComponent& src, MaterialComponent& material, int entityID)
	{
		//TODO default Material
		CC3D_PROFILE_FUNCTION();

		const float textureIndex = 0.0f; // White Texture
		const float tilingFactor = 1.0f;
		std::vector<Mesh>& meshes = src.m_Meshes;
		for (size_t i = 0; i < meshes.size(); i++)
		{
			const size_t TriVertexCount = meshes[i].vertices.size();
			if (s_Data.TriIndexCount >= RendererData::MaxIndices)
				NextBatch();

			for (size_t j = 0; j < TriVertexCount; j++)
			{
				// TODO multiple Textures
				s_Data.TriVertexBufferPtr->Position = glm::vec3(transform * glm::vec4(meshes[i].vertices[j].Position, 1));
				glm::vec3 p = s_Data.TriVertexBufferPtr->Position;
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
