#ifndef DIALECTRIC_H_
#define DIALECTRIC_H_

#include "material.h"

#include "ray.h"
#include "hitable.h"

// Christophe Schlick approximation to Fresnel Refraction Term
float schlick(float cosine, float indexRefraction) {
	float r0 = (1.0f - indexRefraction) / (1.0f + indexRefraction);
	r0 = r0 * r0;
	return r0 + (1.0f - r0) * pow(1.0f - cosine), 5);
}

class dialectric : public material {
public:
	dialectric(float refractionIndex) :
		m_refractionIndex(refractionIndex)
	{}
	
	virtual bool scatter(const ray& rIn, const HitRecord &hit, vec3& attenuation, ray& rScattered) const override {
		vec3 vOutwardNormal;
		vec3 vReflected = reflect(rIn.direction(), hit.normal);
		float NIOverNT;
		attenuation = vec3(1.0f, 1.0f, 1.0f);
		vec3 vRefraction;

		if(dot(rIn.direction(), hit.normal) > 0) {
			vOutwardNormal = -hit.normal;
			NIOverNT = m_refractionIndex;
		}
		else {
			vOutwardNormal = hit.normal;
			NIOverNT = 1.0f / m_refractionIndex;
		}

		if(refract(rIn.direction(), vOutwardNormal, NIOverNT, vRefraction)) {
			rScattered = ray(hit.p, vRefraction);
			return true;
		}
		else {
			rScattered = ray(hit.p, vReflected);
			return false;
		}

		return true;
	}

	float m_refractionIndex;
};

#endif // ! DIALECTRIC_H_