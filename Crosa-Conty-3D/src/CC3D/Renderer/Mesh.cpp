#include "ccpch.h"

#include "CC3D/Renderer/Mesh.h"
#include "CC3D/Renderer/RenderCommand.h"
#include "tiny_obj_loader/tiny_obj_loader.h"

#define TINYOBJLOADER_IMPLEMENTATION

namespace CC3D
{
	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices)
	{
		this->vertices = vertices;
		this->indices = indices;
	}
}