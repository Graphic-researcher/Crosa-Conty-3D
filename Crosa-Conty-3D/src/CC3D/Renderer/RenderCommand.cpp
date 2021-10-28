#include "ccpch.h"
#include "CC3D/Renderer/RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace CC3D {

	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}