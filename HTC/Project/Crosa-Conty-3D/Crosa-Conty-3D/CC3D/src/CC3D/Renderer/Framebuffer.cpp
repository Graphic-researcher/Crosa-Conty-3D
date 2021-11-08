#include "cc3d_pch.h"
#include "CC3D/Renderer/Framebuffer.h"

#include "CC3D/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace CC3D {
	
	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    CC3D_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLFramebuffer>(spec);
		}

		CC3D_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}

