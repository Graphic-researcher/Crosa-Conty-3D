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
    class Model
    {
    public:
        Model(std::string const& path);

    public://TODO move to private
        std::vector<Mesh>    meshes;

    private:
        void loadModel(std::string const& path);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    private:
        std::string m_Path;
        // TODO ‘§¿¿Õº 
        Ref<Texture2D> preView;
    };


    
}

