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
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CC3D_CORE_ASSERT(status, "Failed to initialize Glad!");

		CC3D_CORE_INFO("OpenGL Info:");
		CC3D_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		CC3D_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		CC3D_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));
		CC3D_CORE_INFO("  GLSL_Version: {0}", glGetString(GL_SHADING_LANGUAGE_VERSION));

#ifdef CC3D_ENABLE_ASSERTS
		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		CC3D_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 6), "CC3D requires at least OpenGL version 4.6!");
#endif
	}
	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}