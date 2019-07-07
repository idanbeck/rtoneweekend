#ifndef HITABLE_H_
#define HITABLE_H_

#include "ray.h"

struct HitRecord {
	float t;
	vec3 p;
	vec3 normal;
};

class hitable {
public:
	virtual bool hit(const ray &r, float tMin, float tMax, HitRecord &rec) const = 0;
};

#endif // ! HITABLE_H_