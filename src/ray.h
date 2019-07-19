#ifndef RAY_H_
#define RAY_H_

#include "vec3.h"

class ray {
public:
	ray() {}
	
	ray(const vec3& a, const vec3& b, float ti = 0.0f) {
		m_A = a;
		m_B = b;
		m_time = ti;
	}
	
	vec3 origin() const {
		return m_A;
	}
	
	vec3 direction() const {
		return m_B;
	}
	
	vec3 PointAtParameter(float t) const {
		return m_A + t * m_B;
	}
	
	float time() const {
		return m_time;
	}
	
	vec3 m_A;
	vec3 m_B;
	float m_time;
};

#endif // ! RAY_H_
