#pragma once
#include "CC3D/Renderer/VertexArray.h"

namespace CC3D {

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }
	private:
		uint32_t m_RendererID;

		// VertexBuffers mains mutiBuffers for vertices------
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		// --------------------------------------------------

		// IndexBuffer contains index of each triangle-------
		Ref<IndexBuffer> m_IndexBuffer;
		// --------------------------------------------------
	};

}