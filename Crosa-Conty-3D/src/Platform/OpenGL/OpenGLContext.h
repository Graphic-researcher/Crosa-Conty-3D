#pragma once
#include "CC3D/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace CC3D
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers()override;
	private:
		GLFWwindow* m_WindowHandle;
	};
}