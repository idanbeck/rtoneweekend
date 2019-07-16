#ifndef LAMBERTIAN_H_
#define LAMBERTIAN_H_

#include "material.h"

#include "ray.h"
#include "hitable.h"

#include "texture.h"

class lambertian : public material {
public:
	lambertian(texture *pTexture) :
		m_pAlbedoTexture(pTexture) 
	{}
	
	virtual bool scatter(const ray& rIn, const HitRecord &hit, vec3& attenuation, ray& rScattered) const override {
		vec3 target = hit.p + hit.normal + RandomVectorInUnitSphere();
		rScattered = ray(hit.p, target - hit.p, rIn.m_time);
		attenuation = m_pAlbedoTexture->value(0, 0, hit.p);
		return true;
	}

	texture *m_pAlbedoTexture;
};

#endif // ! LAMBERTIAN_H_
