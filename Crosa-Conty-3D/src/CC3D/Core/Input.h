#pragma once

#include "CC3D/Core/Base.h"
#include "CC3D/Core/KeyCodes.h"
#include "CC3D/Core/MouseCodes.h"
#include <iostream>
#include <glm/glm.hpp>

namespace CC3D {

	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode keycode);

		static bool IsMouseButtonPressed(MouseCode button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}