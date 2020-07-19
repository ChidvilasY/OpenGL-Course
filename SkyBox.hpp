#pragma once

#include <vector>
#include <string>

#include <GL/glew.h>

#include <glm/glm.hpp>

#include "Shader.hpp"
#include "Mesh.hpp"

class SkyBox
{
public:
    SkyBox(std::vector<std::string> faceLocations);

    void Draw(glm::mat4 viewMatrix, glm::mat4 projMatrix);

    ~SkyBox();

private:
    Mesh *mSkyMesh;
    Shader *mShader;

    GLuint mTextureID;
    GLint mUniformProjection;
    GLint mUniformVeiw;
};
