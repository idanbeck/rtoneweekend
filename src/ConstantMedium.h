#ifndef CONSTANT_MEDIUM_H_
#define CONSTANT_MEDIUM_H_

#include "hitable.h"
#include "texture.h"
#include "material.h"

class isotropic : public material {
public:
	isotropic(texture *pTexture) :
	m_pAlbedoTexture(pTexture)
	{
		//
	}
	
	virtual bool scatter(const ray& r, const HitRecord& hit, vec3& attenuation, ray& rScattered) const {
		rScattered = ray(hit.p, RandomVectorInUnitSphere());
		attenuation = m_pAlbedoTexture->value(hit.u, hit.v, hit. p);
		return true;
	}
	
	texture *m_pAlbedoTexture = nullptr;
};

class ConstantMedium : public hitable {
public:
	ConstantMedium(hitable *pHitable, float d, texture *pTexture) :
	m_pHitableBoundary(pHitable),
	m_density(d)
	{
		m_pMaterialPhaseFunction = new isotropic(pTexture);
	}
	
	virtual bool hit(const ray& r, float tMin, float tMax, HitRecord& rec) const override;
	virtual bool BoundingBox(float t0, float t1, aabb& box) const override {
		return m_pHitableBoundary->BoundingBox(t0, t1, box);
	}
	
	hitable *m_pHitableBoundary = nullptr;
	float m_density;
	material *m_pMaterialPhaseFunction = nullptr;
};

bool ConstantMedium::hit(const ray& r, float tMin, float tMax, HitRecord& rec) const {
	bool db = (drand48() < 0.00001f);
	db = false;
	HitRecord hit1, hit2;
	if(m_pHitableBoundary->hit(r, -std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), hit1)) {
		if(m_pHitableBoundary->hit(r, hit1.t + 0.0001f, std::numeric_limits<float>::max(), hit2)) {
			if(db) std::cerr << "\nt0 t1 " << hit1.t << " " << hit2.t << "\n";
			
			if(hit1.t < tMin)
				hit1.t = tMin;
			
			if(hit2.t > tMax)
				hit2.t = tMax;
			
			if(hit1.t >= hit2.t)
				return false;
			
			if(hit1.t < 0)
				hit1.t = 0;
			
			float distanceInBoundary = (hit2.t - hit1.t) * r.direction().length();
			float hitDistance = -(1.0f / m_density) * log(drand48());
			
			if(hitDistance < distanceInBoundary) {
				if(db) std::cerr << "HitDistance = " << hitDistance << "\n";
				
				rec.t = hit1.t + hitDistance / r.direction().length();
				
				if(db) std::cerr << "hit.t = " << rec.t << "\n";
				
				rec.p = r.PointAtParameter(rec.t);
				
				if(db) std::cerr << "hit.p = " << rec.p << "\n";
				
				rec.normal = vec3(1.0f, 0.0f, 0.0f);		// arbitrary?
				rec.pMaterial = m_pMaterialPhaseFunction;
				
				return true;
			}
		}
	}
	return false;
}

#endif // CONSTANT_MEDIUM_H_
