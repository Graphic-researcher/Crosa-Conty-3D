#include "cc3d_pch.h"
#include "Application.h"



/////// Event System //////
#include "CC3D/Events/ApplicationEvent.h"
#include "CC3D/Events/KeyEvent.h"
#include "CC3D/Log.h"
///////////////////////////

#include "glad/glad.h"
#include "GLFW/glfw3.h"


namespace CC3D {
	Application::Application()
	{
	}
	Application::~Application()
	{
	}
	void Application::Run()
	{
		std::cout<<glfwInit()<<std::endl;



		WindowResizeEvent e(1280, 720);
		WindowCloseEvent e1;
		KeyPressedEvent e2(1, 3);
		printf("Window Width: %d\n", e.GetWidth());
		if (e.IsInCategory(EventCategoryApplication))
		{
			/// #include "spdlog/fmt/ostr.h"
			/// this header file could use the ToString
			/// function in your class automatically
			/// if you don't want to include "spdlog/fmt/ostr.h"
			/// you need to write the *.ToString() by yourself as follow
			/// CC3D_TRACE(e.ToString())
			
			//CC3D_TRACE(e.ToString());
			CC3D_TRACE(e);
			CC3D_TRACE(e1);
			CC3D_TRACE(e2);
		}
		if (e.IsInCategory(EventCategoryInput))
		{
			CC3D_TRACE(e);
			CC3D_TRACE(e1);
			CC3D_TRACE(e2);
		}
		if (e2.IsInCategory(EventCategoryKeyboard))
		{
			CC3D_TRACE(e);
			CC3D_TRACE(e1);
			CC3D_TRACE(e2);
		}

		auto window = glfwCreateWindow(100,100,"CC3D",NULL,NULL);
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return;
		}
		glfwMakeContextCurrent(window);
		glViewport(0, 0, 100, 100);
		
		while (!glfwWindowShouldClose(window))
		{
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}
}