#ifndef CHECKER_TEXTURE_H_
#define CHECKER_TEXTURE_H_

#include "texture.h"

class CheckerTexture : public texture {
public:
	CheckerTexture() { }

	CheckerTexture(texture *pTextureEven, texture *pTextureOdd) :
		m_pTextureEven(pTextureEven),
		m_pTextureOdd(pTextureOdd)
	{
		//
	}

	virtual vec3 value(float u, float v, const vec3& p) const override {
		float sines = sin(10.0f * p.e[0]) * sin(10.0f * p.e[1]) * sin(10.0f * p.e[2]);

		if (sines < 0)
			return m_pTextureOdd->value(u, v, p);
		else
			return m_pTextureEven->value(u, v, p);
	}

	texture *m_pTextureEven = nullptr;
	texture* m_pTextureOdd = nullptr;
};

#endif // ! CHECKER_TEXTURE_H_