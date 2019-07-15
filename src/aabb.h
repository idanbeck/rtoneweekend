#ifndef AABB_H_
#define AABB_H_

#include "vec3.h"
#include "ray.h"

// AABB

// These are faster since they don't worry about NaNs
// or exceptions

inline float ffmax(float a, float b) {
	return a > b ? a : b;
}

inline float ffmin(float a, float b) {
	return a < b ? a : b;
}

class aabb {
public:
	aabb() { }
	
	aabb(const vec3& vA, const vec3& vB) :
		m_vMin(vA),
		m_vMax(vB)
	{ }
	
	bool hit(const ray& r, float tmin, float tmax) const {
		for(int a = 0; a < 3; a++) {
			float invD = 1.0f / r.direction()[a];
			float t0 = (m_vMin[a] - r.origin()[a]) * invD;
			float t1 = (m_vMax[a] - r.origin()[a]) * invD;
			
			if(invD < 0.0f)
				std::swap(t0, t1);
			
			tmin = t0 > tmin ? t0 : tmin;
			tmax = t1 < tmax ? t1 : tmax;
			
			if(tmax <= tmin)
				return false;
			
//			float t0 = ffmin(
//			  (m_vMin[a] - r.origin()[a]) / r.direction()[a],
//			  (m_vMax[a] - r.origin()[a]) / r.direction()[a]
//			);
//
//			float t1 = ffmax(
//			  (m_vMin[a] - r.origin()[a]) / r.direction()[a],
//			  (m_vMax[a] - r.origin()[a]) / r.direction()[a]
//			);
//
//			tmin = ffmax(t0, tmin);
//			tmax == ffmin(t1, tmax);
//
//			if(tmax <= tmin)
//				return false;
		}
		return true;
	}
	
	static aabb SurroundingBox(aabb A, aabb B) {
		vec3 vMin(ffmin(A.m_vMin[0], B.m_vMin[0]),
				  ffmin(A.m_vMin[1], B.m_vMin[1]),
				  ffmin(A.m_vMin[2], B.m_vMin[2]));
		
		vec3 vMax(ffmax(A.m_vMax[0], B.m_vMax[0]),
				  ffmax(A.m_vMax[1], B.m_vMax[1]),
				  ffmax(A.m_vMax[2], B.m_vMax[2]));
		
		return aabb(vMin, vMax);
	}
	
	vec3 m_vMin;
	vec3 m_vMax;
};

#endif // ! AABB_H_
