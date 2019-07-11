#ifndef CAMERA_H_
#define CAMERA_H_

#include "ray.h"

vec3 RandomUnitVectorInUnitDisk() {
	vec3 p;

	do {
		p = 2.0f * vec3(drand48(), drand48(), 0.0f) - vec3(1, 1, 0);
	} while (dot(p,p) >= 1.0f);

	return p;
}

class camera {
public:
	camera(vec3 lookFrom, vec3 lookAt, vec3 vUp, float fov, float aspect, float aperture, float focusDistance, float t0, float t1) :
		m_time0(t0),
		m_time1(t1)
	{
		m_lensRadius = aperture / 2.0f;

		float theta = fov * M_PI/180.0f;
		float halfHeight = tan(theta/2.0f);
		float halfWidth = aspect * halfHeight;

		m_vOrigin = lookFrom;
		m_w = UnitVector(lookFrom - lookAt);
		m_u = UnitVector(cross(vUp, m_w));
		m_v = cross(m_w, m_u);

		m_vLowerLeftCorner = m_vOrigin - halfWidth * focusDistance * m_u - halfHeight * focusDistance * m_v - focusDistance * m_w;

		m_vHorizontal = 2.0f * halfWidth * focusDistance * m_u;
		m_vVertical = 2.0f * halfHeight * focusDistance * m_v;
		
	}

	ray getRay(float s, float t) {
		vec3 rd = m_lensRadius * RandomUnitVectorInUnitDisk();
		vec3 vOffset = m_u * rd.x() + m_v * rd.y();
		float time = m_time0 + drand48() * (m_time1 - m_time0);
		return ray(m_vOrigin + vOffset, m_vLowerLeftCorner + s * m_vHorizontal + t * m_vVertical - m_vOrigin - vOffset, time);
	}

	vec3 m_vLowerLeftCorner;
	vec3 m_vHorizontal;
	vec3 m_vVertical;
	vec3 m_vOrigin;
	vec3 m_u;
	vec3 m_v;
	vec3 m_w;
	float m_lensRadius;
	float m_time0, m_time1;
};

#endif // ! CAMERA_H_
