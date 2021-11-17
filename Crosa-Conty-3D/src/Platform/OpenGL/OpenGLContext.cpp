#include "ccpch.h"
#include "Platform/OpenGL/OpenGLContext.h"

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
		CC3D_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CC3D_CORE_ASSERT(status, "Failed to initialize Glad!");

		CC3D_CORE_INFO("OpenGL Info:");
		CC3D_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		CC3D_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		CC3D_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));
		CC3D_CORE_INFO("  GLSL_Version: {0}", glGetString(GL_SHADING_LANGUAGE_VERSION));

		CC3D_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 6), "CC3D requires at least OpenGL version 4.6!");
	}
	void OpenGLContext::SwapBuffers()
	{
		CC3D_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}
}