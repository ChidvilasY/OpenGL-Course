#pragma once

#include <GL/glew.h>

class ShadowMap
{
public:
    ShadowMap();

    virtual bool Init(GLuint width, GLuint height);

    virtual void Write();

    virtual void Read(GLenum textureUnit);

    GLuint GetShadowWidth() { return mShadowWidth; }
    GLuint GetShadowHeight() { return mShadowHeight; }

    ~ShadowMap();

private:
    GLuint mFBO, mShadowMap;
    GLuint mShadowWidth, mShadowHeight;
};
