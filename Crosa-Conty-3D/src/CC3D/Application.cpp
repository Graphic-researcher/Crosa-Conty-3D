#include "ccpch.h"

#include "Application.h"

#include "Events/ApplicationEvent.h"
#include "Log.h"

namespace CC3D {
	Application::Application()
	{
	}

	Application::~Application()
	{
	}
	void Application::Run()
	{

		while (1)
		{
			WindowResizeEvent e(1280, 720);
			if (e.IsInCategory(EventCategoryApplication))
			{
				CC3D_TRACE(e);
			}
			if (e.IsInCategory(EventCategoryInput))
			{
				CC3D_TRACE(e);
			}
			//std::cout << "1" << std::e ndl;
			while (1);
		}
	}
}

