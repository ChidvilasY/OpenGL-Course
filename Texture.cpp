#include <stdexcept>

#include "Texture.hpp"

Texture::Texture(const char *fileLoc)
    : mTextureID(0),
      mWidth(0),
      mHeight(0),
      mBitDepth(0),
      mFileLoc(fileLoc)
{
}

Texture::~Texture()
{
    ClearTexture();
}

bool Texture::LoadTexture()
{
    stbi_uc *imageData = stbi_load(mFileLoc.c_str(), &mWidth, &mHeight, &mBitDepth, STBI_rgb);
    if (imageData == nullptr)
    {
        return false;
    }

    glGenTextures(1, &mTextureID);
    glBindTexture(GL_TEXTURE_2D, mTextureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(imageData);

    return true;
}

bool Texture::LoadTextureA()
{
    stbi_uc *imageData = stbi_load(mFileLoc.c_str(), &mWidth, &mHeight, &mBitDepth, STBI_rgb_alpha);
    if (imageData == nullptr)
    {
        return false;
    }

    glGenTextures(1, &mTextureID);
    glBindTexture(GL_TEXTURE_2D, mTextureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(imageData);

    return true;
}

void Texture::UseTexture()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTextureID);
}

void Texture::ClearTexture()
{
    if (mTextureID)
    {
        glDeleteTextures(1, &mTextureID);
    }

    mTextureID = 0;
    mWidth = 0;
    mHeight = 0;
    mBitDepth = 0;
    mFileLoc = std::string();
}
