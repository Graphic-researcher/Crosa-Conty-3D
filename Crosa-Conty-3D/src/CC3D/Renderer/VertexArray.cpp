#include "ccpch.h"
#include "CC3D/Renderer/VertexArray.h"

#include "CC3D/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace CC3D {

	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    CC3D_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLVertexArray>();
		}

		CC3D_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}