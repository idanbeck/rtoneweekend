#include <iostream>
#include <iostream>
#include <fstream>

#include "vec3.h"
#include "ray.h"

float HitSphere(const vec3& center, float radius, const ray& r) {
	vec3 oc = r.origin() - center;
	
	float a = dot(r.direction(), r.direction());
	float b = 2.0f * dot(oc, r.direction());
	float c = dot(oc, oc) - radius*radius;
	
	float discriminant = b*b - 4*a*c;

	if(discriminant < 0) {
		return -1.0f;
	}
	else {
		return (-b - sqrt(discriminant)) / (2.0f * a);
	} 
}

vec3 color(const ray& r) {

	float t = HitSphere(vec3(0.0f, 0.0f, -1.0f), 0.5f, r);

	if(t > 0.0f) {
		vec3 N = UnitVector(r.PointAtParameter(t) - vec3(0.0f, 0.0f, -1.0f));
		return 0.5f * vec3(N.x() + 1.0f, N.y() + 1.0f, N.z() + 1.0f);
	}

	vec3 vUnitDirection = UnitVector(r.direction());
	t = 0.5f * (vUnitDirection.y() + 1.0f);
	return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
}

int main(int argc, char *argv[]) {
	int nx = 200;
	int ny = 100;

	std::ofstream fileOutput;
  	fileOutput.open("out.ppm");

	fileOutput << "P3\n" << nx << " " << ny << "\n255\n";

	vec3 vLowerLeftCorner(-2.0f, -1.0f, -1.0f);
	vec3 vHorizontal(4.0f, 0.0f, 0.0f);
	vec3 vVertical(0.0f, 2.0f, 0.0f);
	vec3 vOrigin(0.0f, 0.0f, 0.0f);

	for(int j = ny - 1; j >= 0; j--) {
		for(int i = 0; i < nx; i++) {
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);

			ray r(vOrigin, vLowerLeftCorner + u * vHorizontal + v * vVertical);

			vec3 col = color(r);

			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);

			fileOutput << ir << " " << ig << " " << ib << "\n";
		}
	}

	fileOutput.close();

	return 0;
}