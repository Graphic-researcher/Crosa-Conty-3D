#pragma once
#include <vector>
#include "CC3D/Renderer/Mesh.h"

namespace CC3D {

	struct MeshRendererComponent
	{
		Mesh mesh;
		MeshRendererComponent() = default;
		MeshRendererComponent(const MeshRendererComponent&) = default;
	};
}