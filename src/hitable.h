#ifndef HITABLE_H_
#define HITABLE_H_

#include "ray.h"
#include "aabb.h"

class material;

struct HitRecord {
	float t;
	float u;
	float v;
	vec3 p;
	vec3 normal;
	material *pMaterial;
};

class hitable {
public:
	virtual bool hit(const ray &r, float tMin, float tMax, HitRecord &rec) const = 0;
	virtual bool BoundingBox(float t0, float t1, aabb& box) const = 0;

	static int BoxXCompare(const void *pA, const void *pB);
	static int BoxYCompare(const void *pA, const void *pB);
	static int BoxZCompare(const void *pA, const void *pB);
	
protected:
	material *m_pMaterial = nullptr;
};

int hitable::BoxXCompare(const void *pA, const void *pB) {
	aabb boxLeft, boxRight;
	hitable *ah = *(hitable**)pA;
	hitable *bh = *(hitable**)pB;
	
	if(ah->BoundingBox(0, 0, boxLeft) == false || bh->BoundingBox(0, 0, boxRight) == false) {
		std::cerr << "no bounding box in BVH Constructor (X axis compare)\n";
	}
	
	if(boxLeft.m_vMin[0] - boxRight.m_vMin[0] < 0.0f)
		return -1;
	else
		return 1;
}

int hitable::BoxYCompare(const void *pA, const void *pB) {
	aabb boxLeft, boxRight;
	hitable *ah = *(hitable**)pA;
	hitable *bh = *(hitable**)pB;
	
	if(ah->BoundingBox(0, 0, boxLeft) == false || bh->BoundingBox(0, 0, boxRight) == false) {
		std::cerr << "no bounding box in BVH Constructor (Y axis compare)\n";
	}
	
	if(boxLeft.m_vMin[1] - boxRight.m_vMin[1] < 0.0f)
		return -1;
	else
		return 1;
}

int hitable::BoxZCompare(const void *pA, const void *pB) {
	aabb boxLeft, boxRight;
	
	hitable *ah = *(hitable**)pA;
	hitable *bh = *(hitable**)pB;
	
	if(ah->BoundingBox(0, 0, boxLeft) == false || bh->BoundingBox(0, 0, boxRight) == false) {
		std::cerr << "no bounding box in BVH Constructor (Z axis compare)\n";
	}
	
	if(boxLeft.m_vMin[2] - boxRight.m_vMin[2] < 0.0f)
		return -1;
	else
		return 1;
}

class FlipNormals : public hitable {
public:
	FlipNormals(hitable *pHitable) :
		m_pHitable(pHitable)
	{
		//
	}

	virtual bool hit(const ray &r, float tMin, float tMax, HitRecord &rec) const override {
		if(m_pHitable->hit(r, tMin, tMax, rec)) {
			rec.normal = -rec.normal;
			return true;
		}
		else {
			return false;
		}
	}

	virtual bool BoundingBox(float t0, float t1, aabb& box) const override {
		return m_pHitable->BoundingBox(t0, t1, box);
	}

	hitable *m_pHitable = nullptr;
};

#endif // ! HITABLE_H_
