#include <iostream>

#include "ShadowMap.hpp"

ShadowMap::ShadowMap()
    : mFBO(0), mShadowMap(0)
{
}

bool ShadowMap::Init(GLuint width, GLuint height)
{
    mShadowWidth = width;
    mShadowHeight = height;

    glGenFramebuffers(1, &mFBO);

    glGenTextures(1, &mShadowMap);
    glBindTexture(GL_TEXTURE_2D, mShadowMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFBO);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mShadowMap, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "FrameBuffer Error : " << status << std::endl;

        return false;
    }

    return true;
}

void ShadowMap::Write()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFBO);
}

void ShadowMap::Read(GLenum textureUnit)
{
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, mShadowMap);
}

ShadowMap::~ShadowMap()
{
    if (mFBO)
    {
        glDeleteFramebuffers(1, &mFBO);
        mFBO = 0;
    }

    if (mShadowMap)
    {
        glDeleteTextures(1, &mShadowMap);
        mShadowMap = 0;
    }
}
