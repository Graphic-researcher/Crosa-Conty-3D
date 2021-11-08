#include <CC3D.h>
#include <CC3D/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace CC3D {

	class CC3DInput : public Application
	{
	public:
		CC3DInput()
			: Application("CC3D Editor")
		{
			PushLayer(new EditorLayer());
		}

		~CC3DInput()
		{
		}
	};

	Application* CreateApplication()
	{
		return new CC3DInput();
	}

}