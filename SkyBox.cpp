#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"

#include "SkyBox.hpp"

SkyBox::SkyBox(std::vector<std::string> faceLocations)
    : mSkyMesh(new Mesh()),
      mShader(new Shader()),
      mTextureID(0),
      mUniformProjection(0),
      mUniformVeiw(0)
{
    // Init Shader
    mShader->CreateShaderFromFiles("Shaders/skybox.vert", "Shaders/skybox.frag");
    mShader->UseShader();
    mUniformProjection = mShader->GetProjectionLocation();
    mUniformVeiw = mShader->GetViewLocation();

    // Init Texture
    glGenTextures(1, &mTextureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureID);

    int width, height, channels;
    for (GLenum i = 0; i < 6; i++)
    {
        stbi_uc *imageData = stbi_load(faceLocations[i].c_str(), &width, &height, &channels, STBI_rgb);
        if (imageData == nullptr)
        {
            throw std::runtime_error("Failed to load: " + faceLocations[i]);
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

        stbi_image_free(imageData);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Mesh Setup
    GLuint skyBoxIndices[] = {
        // Front
        0, 1, 2,
        2, 1, 3,
        // Right
        2, 3, 5,
        5, 3, 7,
        // Back
        5, 7, 4,
        4, 7, 6,
        // Left
        4, 6, 0,
        0, 6, 1,
        // Top
        4, 0, 5,
        5, 0, 2,
        // Bottom
        1, 6, 3,
        3, 6, 7};

    GLfloat skyBoxVertices[] = {
        // x, y, z   u, v   nx, ny, nz
        -1.f, 1.f, -1.f, 0.f, 0.f, 0.f, 0.f, 0.f,
        -1.f, -1.f, -1.f, 0.f, 0.f, 0.f, 0.f, 0.f,
        1.f, 1.f, -1.f, 0.f, 0.f, 0.f, 0.f, 0.f,
        1.f, -1.f, -1.f, 0.f, 0.f, 0.f, 0.f, 0.f,

        -1.f, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f,
        1.f, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f,
        -1.f, -1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f,
        1.f, -1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f};

    mSkyMesh->CreateMesh(skyBoxVertices, skyBoxIndices, 64, 36); // ?????????????????? 64?
}

SkyBox::~SkyBox()
{
}

void SkyBox::Draw(glm::mat4 viewMatrix, glm::mat4 projMatrix)
{
    viewMatrix = glm::mat4(glm::mat3(viewMatrix));

    glDepthMask(GL_FALSE);

    mShader->UseShader();

    glUniformMatrix4fv(mUniformVeiw, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(mUniformProjection, 1, GL_FALSE, glm::value_ptr(projMatrix));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureID);

    mShader->Validate();

    mSkyMesh->RenderMesh();

    glDepthMask(GL_TRUE);
}
