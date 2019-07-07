#ifndef LAMBERTIAN_H_
#define LAMBERTIAN_H_

#include "material.h"

#include "ray.h"
#include "hitable.h"

class lambertian : public material {
public:
	lambertian(vec3 a) :
		albedo(a) 
	{}
	
	virtual bool scatter(const ray& rIn, const HitRecord &hit, vec3& attenuation, ray& rScattered) const override {
		vec3 target = hit.p + hit.normal + RandomVectorInUnitSphere();
		rScattered = ray(hit.p, target - hit.p);
		attenuation = albedo;
		return true;
	}

	vec3 albedo;
};

#endif // ! LAMBERTIAN_H_