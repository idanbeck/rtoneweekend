#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "vec3.h"

class material {
public:
	virtual bool scatter(const ray& rIn, const HitRecord &hit, vec3& attenuation, ray& rScattered) const = 0;

	virtual vec3 emitted(float u, float v, const vec3& p) const {
		return vec3(0.0f, 0.0f, 0.0f);
	}
};

#endif // ! MATERIAL_H_