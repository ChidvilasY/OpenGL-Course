#pragma once

#include <string>

#include <GL/glew.h>

#include "stb_image.h"

class Texture
{
public:
    Texture(const char* fileLoc);

    bool LoadTexture();
    bool LoadTextureA();
    void UseTexture();
    void ClearTexture();

    ~Texture();

private:
    GLuint mTextureID;
    int mWidth, mHeight, mBitDepth;
    std::string mFileLoc;
};
