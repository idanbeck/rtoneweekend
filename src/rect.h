#ifndef RECT_H_
#define RECT_H_

#include "hitable.h"
#include "material.h"

class XYRect : public hitable {
public:
	XYRect() {}
	XYRect(float x0, float x1, float y0, float y1, float k, material *pMaterial) :
		m_pMaterial(pMaterial),
		m_x0(x0), m_x1(x1), m_y0(y0), m_y1(y1), m_k(k)
	{
		//
	}

	virtual bool hit(const ray &r, float t0, float t1, HitRecord &hit) const override;
	virtual bool BoundingBox(float t0, float t1, aabb& box) const override {
		box = aabb(vec3(m_x0, m_y0, m_k - 0.0001f), vec3(m_x1, m_y1, m_k + 0.0001f));
		return true;
	}

	material *m_pMaterial = nullptr;
	float m_x0 = 0.0f;
	float m_x1 = 0.0f;
	float m_y0 = 0.0f;
	float m_y1 = 0.0f;
	float m_k = 0.0f;
};

bool XYRect::hit(const ray &r, float t0, float t1, HitRecord &hit) const {
	float t = (m_k - r.origin().z()) / r.direction().z();

	// Check Plane
	if( t < t0 || t > t1)
		return false;

	float x = r.origin().x() + t * r.direction().x();
	float y = r.origin().y() + t * r.direction().y();

	// Check rect bounds
	if(x < m_x0 || x > m_x1 || y < m_y0 || y > m_y1)
		return false;

	hit.u = (x - m_x0) / (m_x1 - m_x0);
	hit.v = (y - m_y0) / (m_y1 - m_y0);
	hit.t = t;
	hit.pMaterial = m_pMaterial;
	hit.p = r.PointAtParameter(t);
	hit.normal = vec3(0, 0, 1.0f);

	return true;
}

// XZ 
class XZRect : public hitable {
public:
	XZRect() {}
	XZRect(float x0, float x1, float z0, float z1, float k, material *pMaterial) :
		m_pMaterial(pMaterial),
		m_x0(x0), m_x1(x1), m_z0(z0), m_z1(z1), m_k(k)
	{
		//
	}

	virtual bool hit(const ray &r, float t0, float t1, HitRecord &hit) const override;
	virtual bool BoundingBox(float t0, float t1, aabb& box) const override {
		box = aabb(vec3(m_x0, m_k - 0.0001f, m_z0), vec3(m_x1, m_k + 0.0001f, m_z1));
		return true;
	}

	material *m_pMaterial = nullptr;
	float m_x0 = 0.0f;
	float m_x1 = 0.0f;
	float m_z0 = 0.0f;
	float m_z1 = 0.0f;
	float m_k = 0.0f;
};

bool XZRect::hit(const ray &r, float t0, float t1, HitRecord &hit) const {
	float t = (m_k - r.origin().y()) / r.direction().y();

	// Check Plane
	if( t < t0 || t > t1)
		return false;

	float x = r.origin().x() + t * r.direction().x();
	float z = r.origin().z() + t * r.direction().z();

	// Check rect bounds
	if(x < m_x0 || x > m_x1 || z < m_z0 || z > m_z1)
		return false;

	hit.u = (x - m_x0) / (m_x1 - m_x0);
	hit.v = (z - m_z0) / (m_z1 - m_z0);
	hit.t = t;
	hit.pMaterial = m_pMaterial;
	hit.p = r.PointAtParameter(t);
	hit.normal = vec3(0, 1.0f, 0.0f);

	return true;
}

// YZ 
class YZRect : public hitable {
public:
	YZRect() {}
	YZRect(float y0, float y1, float z0, float z1, float k, material *pMaterial) :
		m_pMaterial(pMaterial),
		m_y0(y0), m_y1(y1), m_z0(z0), m_z1(z1), m_k(k)
	{
		//
	}

	virtual bool hit(const ray &r, float t0, float t1, HitRecord &hit) const override;
	virtual bool BoundingBox(float t0, float t1, aabb& box) const override {
		box = aabb(vec3(m_k - 0.0001f, m_y0, m_z0), vec3(m_k + 0.0001f, m_y1, m_z1));
		return true;
	}

	material *m_pMaterial = nullptr;
	float m_y0 = 0.0f;
	float m_y1 = 0.0f;
	float m_z0 = 0.0f;
	float m_z1 = 0.0f;
	float m_k = 0.0f;
};

bool YZRect::hit(const ray &r, float t0, float t1, HitRecord &hit) const {
	float t = (m_k - r.origin().x()) / r.direction().x();

	// Check Plane
	if( t < t0 || t > t1)
		return false;

	float y = r.origin().y() + t * r.direction().y();
	float z = r.origin().z() + t * r.direction().z();

	// Check rect bounds
	if(y < m_y0 || y > m_y1 || z < m_z0 || z > m_z1)
		return false;

	hit.u = (y - m_y0) / (m_y1 - m_y0);
	hit.v = (z - m_z0) / (m_z1 - m_z0);
	hit.t = t;
	hit.pMaterial = m_pMaterial;
	hit.p = r.PointAtParameter(t);
	hit.normal = vec3(1.0f, 0.0f, 0.0f);

	return true;
}

#endif // RECT_H_
