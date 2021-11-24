#pragma once

#include "CC3D/Core/UUID.h"
namespace CC3D
{
	struct IDComponent
	{
		UUID ID;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
	};
}
