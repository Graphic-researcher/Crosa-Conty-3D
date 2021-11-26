#pragma once
#include <vector>
namespace CC3D {

	struct MeshRendererComponent
	{
		struct TriVertex
		{
			glm::vec3 Position;
			glm::vec3 Normal;
			glm::vec4 Color;
			glm::vec2 TexCoord;
			float TexIndex;
			float TilingFactor;

			// TODO Editor-only
			int EntityID;
		};
		std::vector<TriVertex> vertices;
		MeshRendererComponent() = default;
		MeshRendererComponent(const MeshRendererComponent&) = default;
	};
}