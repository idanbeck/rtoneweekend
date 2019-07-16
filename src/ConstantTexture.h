#ifndef CONSTANT_TEXTURE_H_
#define CONSTANT_TEXTURE_H_

#include "texture.h"

class ConstantTexture : public texture {
public:
    ConstantTexture() { }

    ConstantTexture(vec3 color) :
        m_color(color)
    {
        //
    }

    virtual vec3 value(float u, float v, const vec3& p) const override {
        return m_color;
    }

    vec3 m_color;
};

#endif  // ! CONSTANT_TEXTURE_H_
