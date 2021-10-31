#include "cc3d_pch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace CC3D {

    Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}