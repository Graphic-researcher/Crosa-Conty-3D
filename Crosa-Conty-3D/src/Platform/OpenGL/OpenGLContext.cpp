#include "ccpch.h"
#include "OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//#include <gl/GL.h>

namespace CC3D {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		:m_WindowHandle(windowHandle)
	{
		CC3D_CORE_ASSERT(windowHandle, "Window handle is null!")
	}
	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CC3D_CORE_ASSERT(status, "Failed to initialize Glad!");

		CC3D_CORE_INFO("OpenGL Info:");
		CC3D_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		CC3D_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		CC3D_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));
		CC3D_CORE_INFO("  GLSL_Version: {0}", glGetString(GL_SHADING_LANGUAGE_VERSION));
	}
	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}