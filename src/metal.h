#ifndef METAL_H_
#define METAL_H_

#include "material.h"

#include "ray.h"
#include "hitable.h"

class metal : public material {
public:
	metal(vec3 a, float fuzz) :
		albedo(a),
		m_fuzz(fuzz)
	{
		if(m_fuzz > 1.0f)
			m_fuzz = 1.0f;
	}
	
	virtual bool scatter(const ray& rIn, const HitRecord &hit, vec3& attenuation, ray& rScattered) const override {
		vec3 vReflected = reflect(UnitVector(rIn.direction()), hit.normal);
		rScattered = ray(hit.p, vReflected + m_fuzz * RandomVectorInUnitSphere());
		attenuation = albedo;
		return (dot(rScattered.direction(), hit.normal) > 0);
	}

	float m_fuzz = 0.0f;
	vec3 albedo;
};

#endif // ! METAL_H_