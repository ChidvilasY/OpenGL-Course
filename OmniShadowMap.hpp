#pragma once

#include "ShadowMap.hpp"

class OmniShadowMap : public ShadowMap
{
public:
    OmniShadowMap();

    virtual bool Init(GLsizei width, GLsizei height);

    virtual void Write();

    virtual void Read(GLenum textureUnit);

    virtual ~OmniShadowMap();

private:
};
