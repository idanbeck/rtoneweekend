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
	camera(vec3 lookFrom, vec3 lookAt, vec3 vUp, float fov, float aspect, float aperture, float focusDistance) {
		lensRadius = aperture / 2.0f;

		float theta = fov * M_PI/180.0f;
		float halfHeight = tan(theta/2.0f);
		float halfWidth = aspect * halfHeight;

		vOrigin = lookFrom;
		w = UnitVector(lookFrom - lookAt);
		u = UnitVector(cross(vUp, w));
		v = cross(w, u);

		vLowerLeftCorner = vOrigin - halfWidth * focusDistance * u - halfHeight * focusDistance * v - focusDistance * w;

		vHorizontal = 2.0f * halfWidth * focusDistance * u;
		vVertical = 2.0f * halfHeight * focusDistance * v;
		
	}

	ray getRay(float s, float t) {
		vec3 rd = lensRadius * RandomUnitVectorInUnitDisk();
		vec3 vOffset = u * rd.x() + v * rd.y();
		return ray(vOrigin + vOffset, vLowerLeftCorner + s * vHorizontal + t * vVertical - vOrigin - vOffset);
	}

	vec3 vLowerLeftCorner;
	vec3 vHorizontal;
	vec3 vVertical;
	vec3 vOrigin;
	vec3 u, v, w;
	float lensRadius;
};

#endif // ! CAMERA_H_