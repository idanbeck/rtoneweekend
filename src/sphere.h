#ifndef SPHERE_H_
#define SPHERE_H_

#include "hitable.h"

class sphere : public hitable {
public:
	sphere() {}
	sphere(vec3 cen, float r) : 
		center(cen),
		radius(r)
	{ }

	virtual bool hit(const ray& r, float tMin, float tMax, HitRecord &rec) const override;

	vec3 center;
	float radius;
};

bool sphere::hit(const ray& r, float tMin, float tMax, HitRecord &rec) const {
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float c = dot(oc, oc) - radius*radius;
	float discriminant = b*b - a*c;
	
	if(discriminant > 0) {
		float sqrtDisc = sqrt(discriminant);
		
		float temp = (-b - sqrtDisc) / a;
		if(temp < tMax && temp > tMin) {
			rec.t = temp;
			rec.p = r.PointAtParameter(temp);
			rec.normal = (rec.p - center) / radius;
			return true;
		}
		
		temp = (-b + sqrtDisc) / a;
		if(temp < tMax && temp > tMin) {
			rec.t = temp;
			rec.p = r.PointAtParameter(temp);
			rec.normal = (rec.p - center) / radius;
			return true;
		}
	}
	
	return false;
}

#endif // ! SPHERE_H_