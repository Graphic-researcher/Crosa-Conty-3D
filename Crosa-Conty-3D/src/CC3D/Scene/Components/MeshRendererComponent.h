#pragma once
#include <vector>
#include "CC3D/Renderer/Mesh.h"

namespace CC3D {

	struct MeshRendererComponent
	{
		std::vector<Mesh> m_Meshes;

		size_t MeshNum = 0;
		
		MeshRendererComponent() = default;
		MeshRendererComponent(const MeshRendererComponent&) = default;
		MeshRendererComponent(std::vector<Mesh>& meshes) :m_Meshes(meshes) { MeshNum = m_Meshes.size(); }
	};
}