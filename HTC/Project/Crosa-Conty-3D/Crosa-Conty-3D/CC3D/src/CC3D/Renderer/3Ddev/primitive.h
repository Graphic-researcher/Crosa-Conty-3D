#pragma once
#include "CC3D/Core/Core.h"
#include "CC3D/Renderer/VertexArray.h"
#include "CC3D/Renderer/Texture.h"
#include "CC3D/Renderer/Shader.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/hash.hpp>

#include <unordered_map>
#include <vector>

namespace CC3D
{
	struct Vertex
	{
		glm::vec3 m_Position;
		glm::vec2 m_TexCoord;
		glm::vec3 m_Normal;
		glm::vec3 m_Tangent;
		glm::vec3 m_Bitangent;
		bool operator == (const Vertex& v) const
		{
			return m_Position == v.m_Position &&
				m_TexCoord == v.m_TexCoord &&
				m_Normal == v.m_Normal;
		};
	};

	struct Posture
	{
		glm::vec3 m_Position;
		glm::vec3 m_Rotation;
		glm::vec3 m_Scale;
		glm::mat4 m_Transform;
		void Update()
		{
			glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_Position);
			glm::mat4 rotation = glm::mat4_cast(glm::qua<float>(glm::radians(m_Rotation)));
			m_Transform = glm::scale(translation * rotation, m_Scale);
		};
	};

	struct Triangle
	{
		uint32_t idx1;
		uint32_t idx2;
		uint32_t idx3;
	};

	enum class MeshType
	{
		None = 0, Plane = 1, Cube = 2, Sphere = 3, Model=4
	};
}

namespace std {
	template<> struct hash<CC3D::Vertex> {
		size_t operator()(CC3D::Vertex const& vertex) const
		{
			return ((hash<glm::vec3>()(vertex.m_Position) ^
				(hash<glm::vec3>()(vertex.m_Normal) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.m_TexCoord) << 1);
		}
	};
}

#define MAX_TEXTURE_NUM 10