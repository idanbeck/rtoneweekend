#ifndef DIFFUSE_LIGHT_H_
#define DIFFUSE_LIGHT_H_

#include "material.h"
#include "texture.h"
#include "ray.h"
#include "hitable.h"

class DiffuseLight : public material {
public:
	DiffuseLight(texture *pEmitTexture) :
		m_pEmitTexture(pEmitTexture)
	{
		//
	}

	virtual bool scatter(const ray& r, const HitRecord& hit, vec3& attenuation, ray& rScattered) const {
		return false;
	}

	virtual vec3 emitted(float u, float v, const vec3& p) const override {
		return m_pEmitTexture->value(u, v, p);
	}

	texture *m_pEmitTexture = nullptr;

};

#endif // DIFFUSE_LIGHT_H_