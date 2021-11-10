#pragma once
#include <CC3D.h>
#include <CC3D/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace CC3D {

	class CC3DEditor : public Application
	{
	public:
		CC3DEditor()
			: Application("CC3D Editor")
		{
			PushLayer(new EditorLayer());
		}

		~CC3DEditor()
		{
		}
	};

	Application* CreateApplication()
	{
		return new CC3DEditor();
	}

}