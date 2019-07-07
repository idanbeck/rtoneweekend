#ifndef CAMERA_H_
#define CAMERA_H_

#include "ray.h"

class camera {
public:
	camera(vec3 lookFrom, vec3 lookAt, vec3 vUp, float fov, float aspect) {
		vec3 u, v, w;

		float theta = fov * M_PI/180.0f;
		float halfHeight = tan(theta/2.0f);
		float halfWidth = aspect * halfHeight;

		vOrigin = lookFrom;
		w = UnitVector(lookFrom - lookAt);
		u = UnitVector(cross(vUp, w));
		v = cross(w, u);

		vLowerLeftCorner = vOrigin - halfWidth * u - halfHeight * v - w;
		vHorizontal = 2.0f * halfWidth * u;
		vVertical = 2.0f * halfHeight * v;
		
	}

	ray getRay(float u, float v) {
		return ray(vOrigin, vLowerLeftCorner + u * vHorizontal + v * vVertical - vOrigin);
	}

	vec3 vLowerLeftCorner;
	vec3 vHorizontal;
	vec3 vVertical;
	vec3 vOrigin;
};

#endif // ! CAMERA_H_