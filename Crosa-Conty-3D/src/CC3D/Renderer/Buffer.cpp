#include "ccpch.h"
#include "CC3D/Renderer/Buffer.h"

#include "CC3D/Renderer/RenderContext.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace CC3D {

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (RenderContext::GetAPI())
		{
			case RendererAPI::API::None:    CC3D_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLVertexBuffer>(size);
		}

		CC3D_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (RenderContext::GetAPI())
		{
			case RendererAPI::API::None:			CC3D_CORE_ASSERT(false, "RendererAPI:None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:			return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}

		CC3D_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (RenderContext::GetAPI())
		{
			case RendererAPI::API::None:			CC3D_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:			return CreateRef<OpenGLIndexBuffer>(indices, count);
		}

		CC3D_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}