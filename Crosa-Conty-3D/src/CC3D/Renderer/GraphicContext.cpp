#include "ccpch.h"
#include "CC3D/Renderer/GraphicsContext.h"

#include "CC3D/Renderer/RenderContext.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace CC3D {

	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (RenderContext::GetAPI())
		{
		case RendererAPI::API::None:    CC3D_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		CC3D_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}