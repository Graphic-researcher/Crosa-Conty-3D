#pragma once
#include "CC3D/Renderer/Texture.h"
#include "CC3D/Renderer/Shader.h"

namespace CC3D
{
#define MAX_BONE_INFLUENCE 4
    struct Vertex {
        // position
        glm::vec3 Position;
        // normal
        glm::vec3 Normal;
        // texCoords
        glm::vec2 TexCoords;
        // tangent
        glm::vec3 Tangent;
        // bitangent
        glm::vec3 Bitangent;
        // TODO  ������
        int m_BoneIDs[MAX_BONE_INFLUENCE];
        // TODO ����Ȩ��
        float m_Weights[MAX_BONE_INFLUENCE];
    };

    class Mesh {
    public:
        Mesh() = default;
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
        ~Mesh() = default;

    public://TODO move to private 

        std::vector<Vertex>       vertices;
        std::vector<uint32_t> indices; //TODO remove
  
    private:
    };
}
