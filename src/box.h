#ifndef BOX_H_
#define BOX_H_

#include "material.h"
#include "hitable.h"
#include "HitableList.h"
#include "rect.h"
#include "vec3.h"

class box : public hitable {
public:
	box() {}
	box(const vec3& p0, const vec3& p1, material *pMaterial);
	
	virtual bool hit(const ray& r, float tMin, float tMax, HitRecord &hit) const override;
	virtual bool BoundingBox(float tMin, float tMax, aabb& aabbBox) const override {
		aabbBox = aabb(m_pMin, m_pMax);
		return true;
	}
	
	vec3 m_pMin;
	vec3 m_pMax;
	hitable *m_pList = nullptr;
};

box::box(const vec3& p0, const vec3& p1, material *pMaterial) :
	m_pMin(p0),
	m_pMax(p1)
{
	m_pMaterial = pMaterial;
	
	hitable **list = new hitable*[6];
	int i = 0;
	list[i++] = new XYRect(m_pMin.x(), m_pMax.x(), m_pMin.y(), m_pMax.y(), m_pMax.z(), m_pMaterial);
	list[i++] = new FlipNormals(new XYRect(m_pMin.x(), m_pMax.x(), m_pMin.y(), m_pMax.y(), m_pMin.z(), m_pMaterial));
	
	list[i++] = new XZRect(m_pMin.x(), m_pMax.x(), m_pMin.z(), m_pMax.z(), m_pMax.y(), m_pMaterial);
	list[i++] = new FlipNormals(new XZRect(m_pMin.x(), m_pMax.x(), m_pMin.z(), m_pMax.z(), m_pMin.y(), m_pMaterial));
	
	list[i++] = new YZRect(m_pMin.y(), m_pMax.y(), m_pMin.z(), m_pMax.z(), m_pMax.x(), m_pMaterial);
	list[i++] = new FlipNormals(new YZRect(m_pMin.y(), m_pMax.y(), m_pMin.z(), m_pMax.z(), m_pMin.x(), m_pMaterial));
	
	m_pList = new HitableList(list, i);
}

bool box::hit(const ray& r, float tMin, float tMax, HitRecord &hit) const {
	return m_pList->hit(r, tMin, tMax, hit);
}

#endif // BOX_H_
