#ifndef DIALECTRIC_H_
#define DIALECTRIC_H_

#include <math.h>

#include "material.h"

#include "ray.h"
#include "hitable.h"

#include "drand48.h"

// Christophe Schlick approximation to Fresnel Refraction Term
float schlick(float cosine, float indexRefraction) {
	float r0 = (1.0f - indexRefraction) / (1.0f + indexRefraction);
	r0 = r0 * r0;
	return r0 + (1.0f - r0) * pow((1.0f - cosine), 5);
}

class dialectric : public material {
public:
	dialectric(float refractionIndex) :
		m_refractionIndex(refractionIndex)
	{
		// empty
	}
	
	virtual bool scatter(const ray& rIn, const HitRecord &hit, vec3& attenuation, ray& rScattered) const override {
		vec3 vOutwardNormal;
		vec3 vReflected = reflect(rIn.direction(), hit.normal);
		float NIOverNT;
		attenuation = vec3(1.0f, 1.0f, 1.0f);
		vec3 vRefraction;
		float reflectProbability;
		float cosine;

		if(dot(rIn.direction(), hit.normal) > 0) {
			vOutwardNormal = -hit.normal;
			NIOverNT = m_refractionIndex;
			cosine = m_refractionIndex * dot(rIn.direction(), hit.normal) / rIn.direction().length();
		}
		else {
			vOutwardNormal = hit.normal;
			NIOverNT = 1.0f / m_refractionIndex;
			cosine = -dot(rIn.direction(), hit.normal) / rIn.direction().length();
		}

		if(refract(rIn.direction(), vOutwardNormal, NIOverNT, vRefraction)) {
			reflectProbability = schlick(cosine, m_refractionIndex);
		}
		else {
			rScattered = ray(hit.p, vReflected, rIn.m_time);
			reflectProbability = 1.0f;
		}

		if(drand48() < reflectProbability) {
			rScattered = ray(hit.p, vReflected, rIn.m_time);
		}
		else {
			rScattered = ray(hit.p, vRefraction, rIn.m_time);
		}
		
		return true;
	}

	float m_refractionIndex;
};

#endif // ! DIALECTRIC_H_
