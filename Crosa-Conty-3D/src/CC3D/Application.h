#pragma once

#include "Core.h"

namespace CC3D {

	class CC3D_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	Application* CreateApplication();
}

