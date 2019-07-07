#ifndef CAMERA_H_
#define CAMERA_H_

#include "ray.h"

class camera {
public:
	camera() {
		vLowerLeftCorner = vec3(-2.0f, -1.0f, -1.0f);
		vHorizontal = vec3(4.0f, 0.0f, 0.0f);
		vVertical = vec3(0.0f, 2.0f, 0.0f);
		vOrigin = vec3(0.0f, 0.0f, 0.0f);
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