#ifndef HITABLE_H_
#define HITABLE_H_

#include "ray.h"

class material;

struct HitRecord {
	float t;
	vec3 p;
	vec3 normal;
	material *pMaterial;
};

class hitable {
public:
	virtual bool hit(const ray &r, float tMin, float tMax, HitRecord &rec) const = 0;

protected:
	material *m_pMaterial = nullptr;
};

#endif // ! HITABLE_H_