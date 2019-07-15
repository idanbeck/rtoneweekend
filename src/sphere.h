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
		m_velocity = vec3(0.0f);
	}

	virtual bool hit(const ray& r, float tMin, float tMax, HitRecord &rec) const override;
	virtual bool BoundingBox(float t0, float t1, aabb& box) const override;
	
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

bool sphere::BoundingBox(float t0, float t1, aabb& box) const {
	
	vec3 hv;
	vec3 center = m_center;
	
	if(m_velocity.IsZero() == false) {
		vec3 centerT0 = m_center;
		vec3 centerT1 = m_center + (t1 - t0) * m_velocity;
		
		center = m_center + 0.5f * (centerT1 - centerT0);
		hv = vec3(ffmax(centerT0[0] + m_radius, centerT1[0] + m_radius),
					   ffmax(centerT0[1] + m_radius, centerT1[1] + m_radius),
					   ffmax(centerT0[2] + m_radius, centerT1[2] + m_radius));
	}
	else {
		hv = vec3(m_radius, m_radius, m_radius);
	}
	
	box = aabb(center - hv, center + hv);
	
	return true;
}

#endif // ! SPHERE_H_
