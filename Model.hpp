#pragma once

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.hpp"
#include "Texture.hpp"

class Model
{
public:
    Model();

    void LoadModel(const std::string fileName);

    void RenderModel();
    void ClearModel();

    ~Model();

private:
    void LoadNode(const aiNode *node, const aiScene *scene);
    void LoadMesh(const aiMesh *mesh, const aiScene *scene);
    void LoadMaterial(const aiScene *scene);

    std::vector<Mesh *> mMeshList{};
    std::vector<Texture *> mTextureList{};
    std::vector<unsigned int> mMeshToTex{};
};
