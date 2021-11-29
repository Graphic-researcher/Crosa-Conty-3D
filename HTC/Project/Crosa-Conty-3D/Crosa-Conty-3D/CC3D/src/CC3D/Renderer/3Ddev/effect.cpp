#include "cc3d_pch.h"
#include "CC3D/Renderer/RendererAPI.h"
#include "CC3D/Renderer/Renderer.h"
#include "CC3D/Renderer/3Ddev/effect.h"
#include "CC3D/Renderer/3Ddev/Platform/opengl_effect.h"

namespace CC3D
{
	Ref<Cubemap> Cubemap::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			CC3D_CORE_ASSERT(false, "RendererAPI::API::None is not currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLCubemap>(path);
		default:
			break;
		}
		CC3D_CORE_ASSERT(false, "Unknown RendererAPI::API!");
		return nullptr;
	}

	Ref<Cubemap> Cubemap::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			CC3D_CORE_ASSERT(false, "RendererAPI::API::None is not currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLCubemap>();
		default:
			break;
		}
		CC3D_CORE_ASSERT(false, "Unknown RendererAPI::API!");
		return nullptr;
	}
}