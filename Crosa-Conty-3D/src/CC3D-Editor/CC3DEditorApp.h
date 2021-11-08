#pragma once
#include <CC3D.h>
#include <CC3D/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace CC3D {

	class HazelEditor : public Application
	{
	public:
		HazelEditor()
			: Application("CC3D Editor")
		{
			PushLayer(new EditorLayer());
		}

		~HazelEditor()
		{
		}
	};

	Application* CreateApplication()
	{
		return new HazelEditor();
	}

}