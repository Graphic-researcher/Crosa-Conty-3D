#include "ccpch.h"
#include "CC3D/Renderer/Texture.h"

#include "CC3D/Renderer/RenderContext.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace CC3D {

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (RenderContext::GetAPI())
		{
			case RendererAPI::API::None:		CC3D_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:		return CreateRef<OpenGLTexture2D>(width, height);
		}

		CC3D_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (RenderContext::GetAPI())
		{
			case RendererAPI::API::None:		CC3D_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:		return CreateRef<OpenGLTexture2D>(path);
		}

		CC3D_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}