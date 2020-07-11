#include <iostream>

#include "OmniShadowMap.hpp"

OmniShadowMap::OmniShadowMap()
    : ShadowMap()
{
}

bool OmniShadowMap::Init(GLsizei width, GLsizei height)
{
    mShadowWidth = width;
    mShadowHeight = height;

    glGenFramebuffers(1, &mFBO);

    glGenTextures(1, &mShadowMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mShadowMap);

    for (GLenum i = 0; i < 6; i++)
    { // for each face of cube
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, mShadowWidth, mShadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mShadowMap, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << __FILE__ << " FrameBuffer Error : " << status << std::endl;

        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return true;
}

void OmniShadowMap::Write()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFBO);
}

void OmniShadowMap::Read(GLenum textureUnit)
{
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mShadowMap);
}

OmniShadowMap::~OmniShadowMap()
{
}
