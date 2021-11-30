#pragma once

#include "CC3D/Renderer/Texture.h"
#include "CC3D/Renderer/Mesh.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stb_image.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace CC3D
{
    struct MeshTree
    {
        std::vector<Mesh> m_Meshes;
        MeshTree* m_Parent = nullptr;
        std::vector<Ref<MeshTree>> m_Children;
        MeshTree() = default;
        MeshTree(Mesh mesh) { m_Meshes.push_back(mesh); }
        MeshTree(MeshTree* parent, Mesh mesh) :m_Parent(parent) { m_Meshes.push_back(mesh); }

        void AddMesh(Mesh mesh)
        {
            m_Meshes.push_back(mesh);
        }

        void AddSubMesh(Ref<MeshTree> subMesh)
        {
            subMesh->m_Parent = this;
            m_Children.push_back(subMesh);
        }
        bool HasChild()
        {
            return  m_Children.size() > 0 ? true : false;
        }
    };

    class Model
    {
    public:
        Model(std::string const& path);

    public://TODO move to private
        Ref<MeshTree> meshes;
        //std::vector<Mesh>    meshes;

    private:
        void loadModel(std::string const& path);
        void processNode(aiNode* node, const aiScene* scene, Ref<MeshTree> currentMeshes);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    private:
        std::string m_Path;
        // TODO ‘§¿¿Õº 
        Ref<Texture2D> preView;
    };


    
}

