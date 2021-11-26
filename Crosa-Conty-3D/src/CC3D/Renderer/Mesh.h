#pragma once
#include "CC3D/Renderer/Texture.h"
#include "CC3D/Renderer/Shader.h"

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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
        // TODO  ¹Ç÷ÀÊý
        int m_BoneIDs[MAX_BONE_INFLUENCE];
        // TODO ¹Ç÷ÀÈ¨ÖØ
        float m_Weights[MAX_BONE_INFLUENCE];
    };

    class Mesh {
    public:
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
        ~Mesh() = default;

    public://TODO move to private
        std::vector<Vertex>       vertices;
        std::vector<unsigned int> indices;
  
    private:
    };
}
