#pragma once

#include <GL/glew.h>

class ShadowMap
{
public:
    ShadowMap();

    virtual bool Init(GLsizei width, GLsizei height);

    virtual void Write();

    virtual void Read(GLenum textureUnit);

    GLsizei GetShadowWidth() { return mShadowWidth; }
    GLsizei GetShadowHeight() { return mShadowHeight; }

    virtual ~ShadowMap();

protected:
    GLuint mFBO, mShadowMap;
    GLsizei mShadowWidth, mShadowHeight;
};
