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

// Transformations

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

class Translate : public hitable {
public:
	Translate(hitable *pHitable, const vec3& vDisplacement) :
		m_pHitable(pHitable),
		m_vOffset(vDisplacement)
	{
		//
	}
	virtual bool hit(const ray &r, float tMin, float tMax, HitRecord &rec) const override;
	virtual bool BoundingBox(float t0, float t1, aabb& box) const override;
	
	hitable *m_pHitable = nullptr;
	vec3 m_vOffset;
};

bool Translate::hit(const ray &r, float tMin, float tMax, HitRecord &rec) const {
	ray rMoved(r.origin() - m_vOffset, r.direction(), r.time());
	if(m_pHitable->hit(rMoved, tMin, tMax, rec)) {
		rec.p += m_vOffset;
		return true;
	}
	else {
		return false;
	}
}

bool Translate::BoundingBox(float t0, float t1, aabb& box) const {
	if(m_pHitable->BoundingBox(t0, t1, box)) {
		box = aabb(box.min() + m_vOffset, box.max() + m_vOffset);
		return true;
	}
	else {
		return false;
	}
}

class RotateY : public hitable {
public:
	RotateY(hitable *pHitable, float degrees) :
		m_pHitable(pHitable)
	{
		m_theta = (M_PI / 180.0f) * degrees;
		m_SinTheta = sin(m_theta);
		m_CosTheta = cos(m_theta);
		m_fHasBox = m_pHitable->BoundingBox(0.0f, 1.0f, m_aabbBox);
		
		vec3 vMin=(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		vec3 vMax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
		
		// This determines the AABB of the rotated entity
		for(int i = 0; i < 2; i++) {
			for(int j = 0; j < 2; j++) {
				for(int k = 0; k < 2; k++) {
					float x = i * m_aabbBox.max().x() + (1 - i) * m_aabbBox.min().x();
					float y = j * m_aabbBox.max().y() + (1 - j) * m_aabbBox.min().y();
					float z = k * m_aabbBox.max().z() + (1 - k) * m_aabbBox.min().z();
					
					float newX =  m_CosTheta * x + m_SinTheta * z;
					float newZ = -m_SinTheta * x + m_CosTheta * z;
					
					vec3 vTester(newX, y, newZ);
					for(int c = 0; c < 3; c++) {
						if(vTester[c] > vMax[c])
							vMax[c] = vTester[c];
						
						if(vTester[c] < vMin[c])
							vMin[c] = vTester[c];
					}
				}
			}
		}
		
		m_aabbBox = aabb(vMin, vMax);
	}
	
	virtual bool hit(const ray &r, float tMin, float tMax, HitRecord &rec) const override {
		vec3 pOrigin = r.origin();
		vec3 vDirection = r.direction();
		
		pOrigin[0] = m_CosTheta * r.origin().x() - m_SinTheta * r.origin().z();
		pOrigin[2] = m_SinTheta * r.origin().x() + m_CosTheta * r.origin().z();
		
		vDirection[0] = m_CosTheta * r.direction().x() - m_SinTheta * r.direction().z();
		vDirection[2] = m_SinTheta * r.direction().x() + m_CosTheta * r.direction().z();
		
		ray rRotated(pOrigin, vDirection, r.time());
		
		if(m_pHitable->hit(rRotated, tMin, tMax, rec)) {
			vec3 p = rec.p;
			vec3 vNormal = rec.normal;
			
			p[0] = m_CosTheta * rec.p.x() - m_SinTheta * rec.p.z();
			p[2] = m_SinTheta * rec.p.x() - m_SinTheta * rec.p.z();
			
			vNormal[0] =  m_CosTheta * rec.normal.x() + m_SinTheta * rec.normal.z();
			vNormal[2] = -m_SinTheta * rec.normal.x() + m_SinTheta * rec.normal.z();
			
			rec.p = p;
			rec.normal = vNormal;
			
			return true;
		}
		else {
			return false;
		}
	}
	
	virtual bool BoundingBox(float t0, float t1, aabb& box) const override {
		box = m_aabbBox;
		return m_fHasBox;
	}
	
	hitable *m_pHitable = nullptr;
	float m_theta;
	bool m_fHasBox = false;
	aabb m_aabbBox;
	
private:
	float m_SinTheta;
	float m_CosTheta;
};

#endif // ! HITABLE_H_
