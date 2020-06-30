#include <iostream>

#include "Model.hpp"

Model::Model() {}
Model::~Model() { ClearModel(); }

void Model::LoadModel(const std::string fileName)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

    if (scene == nullptr)
    {
        throw std::runtime_error("Failed to load: (" + fileName + ") + " + importer.GetErrorString());
    }

    LoadNode(scene->mRootNode, scene);

    LoadMaterial(scene);
}

void Model::RenderModel()
{
    for (size_t i = 0; i < mMeshList.size(); i++)
    {
        GLuint materialIdx = mMeshToTex[i];

        if (materialIdx < mTextureList.size() && mTextureList[materialIdx])
        {
            mTextureList[materialIdx]->UseTexture();
        }

        mMeshList[i]->RenderMesh();
    }
}

void Model::ClearModel()
{
    for (size_t i = 0; i < mMeshList.size(); i++)
    {
        if (mMeshList[i])
        {
            delete mMeshList[i];
            mMeshList[i] = nullptr;
        }
    }

    for (size_t i = 0; i < mTextureList.size(); i++)
    {
        if (mTextureList[i])
        {
            delete mTextureList[i];
            mTextureList[i] = nullptr;
        }
    }
}

void Model::LoadNode(const aiNode *node, const aiScene *scene)
{
    for (size_t i = 0; i < node->mNumMeshes; i++)
    {
        LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
    }

    for (size_t i = 0; i < node->mNumChildren; i++)
    {
        LoadNode(node->mChildren[i], scene);
    }
}

void Model::LoadMesh(const aiMesh *mesh, const aiScene *scene)
{
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    for (size_t i = 0; i < mesh->mNumVertices; i++)
    {
        vertices.insert(vertices.end(), {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z});
        if (mesh->mTextureCoords[0])
        {
            vertices.insert(vertices.end(), {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y});
        }
        else
        {
            vertices.insert(vertices.end(), {0.f, 0.f});
        }

        vertices.insert(vertices.end(), {-mesh->mNormals[i].x, -mesh->mNormals[i].y, -mesh->mNormals[i].z});
    }

    for (size_t i = 0; i < mesh->mNumFaces; i++)
    {
        const aiFace *face = &mesh->mFaces[i];

        for (size_t j = 0; j < face->mNumIndices; j++)
        {
            indices.push_back(face->mIndices[j]);
        }
    }

    Mesh *newMesh = new Mesh();
    newMesh->CreateMesh(vertices.data(), indices.data(), static_cast<unsigned int>(vertices.size()), static_cast<unsigned int>(indices.size()));
    mMeshList.push_back(newMesh);
    mMeshToTex.push_back(mesh->mMaterialIndex);
}

void Model::LoadMaterial(const aiScene *scene)
{
    mTextureList.resize(scene->mNumMaterials);

    for (size_t i = 0; i < scene->mNumMaterials; i++)
    {
        const aiMaterial *material = scene->mMaterials[i];

        mTextureList[i] = nullptr;

        if (material->GetTextureCount(aiTextureType_DIFFUSE))
        {
            aiString path;
            if ((material->GetTexture(aiTextureType_DIFFUSE, 0, &path)) == AI_SUCCESS)
            {
                std::size_t idx = std::string(path.data).rfind("\\");
                std::string fileName = std::string(path.data).substr(idx + 1);

                std::string texPath = "Textures/" + fileName;
                mTextureList[i] = new Texture(texPath.c_str());

                if (!mTextureList[i]->LoadTextureA())
                {
                    std::cout << "Failed to load texture at: " << texPath << std::endl;

                    delete mTextureList[i];
                    mTextureList[i] = nullptr;
                }
            }
        }

        if (!mTextureList[i])
        {
            mTextureList[i] = new Texture("Textures/plain.jpg");
            if (!mTextureList[i]->LoadTextureA())
            {
                throw std::runtime_error("Failed to load texture: ( Textures/plain.jpg )");
            }
        }
    }
}
