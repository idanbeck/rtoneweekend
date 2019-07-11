#ifndef SPHERE_H_
#define SPHERE_H_

#include "hitable.h"

#include "material.h"

class sphere : public hitable {
public:
	sphere() {}
	sphere(vec3 cen, float r, material *pMaterial) : 
		m_center(cen),
		m_radius(r)
	{ 
		m_pMaterial = pMaterial;
	}

	virtual bool hit(const ray& r, float tMin, float tMax, HitRecord &rec) const override;
	void SetVelocity(vec3 v){
		m_velocity = v;
	}

	vec3 m_center;
	float m_radius;
	vec3 m_velocity;
};

bool sphere::hit(const ray& r, float tMin, float tMax, HitRecord &rec) const {
	vec3 oc = r.origin() - m_center;
	
	// Add velocity if it's non-zero
	if(m_velocity.IsZero() == false) {
		oc = (r.origin() + r.m_time * m_velocity) - m_center;
	}
	
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float c = dot(oc, oc) - m_radius*m_radius;
	float discriminant = b*b - a*c;
	
	if(discriminant > 0) {
		float sqrtDisc = sqrt(discriminant);
		
		float temp = (-b - sqrtDisc) / a;
		if(temp < tMax && temp > tMin) {
			rec.t = temp;
			rec.p = r.PointAtParameter(temp);
			rec.normal = (rec.p - m_center) / m_radius;
			rec.pMaterial = m_pMaterial;
			return true;
		}
		
		temp = (-b + sqrtDisc) / a;
		if(temp < tMax && temp > tMin) {
			rec.t = temp;
			rec.p = r.PointAtParameter(temp);
			rec.normal = (rec.p - m_center) / m_radius;
			rec.pMaterial = m_pMaterial;
			return true;
		}
	}
	
	return false;
}

#endif // ! SPHERE_H_
