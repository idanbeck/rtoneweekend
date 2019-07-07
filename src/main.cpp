#include <iostream>
#include <iostream>
#include <fstream>

#include "vec3.h"
#include "ray.h"
#include "float.h"
#include "HitableList.h"
#include "sphere.h"

vec3 color(const ray& r, hitable *world) {
	HitRecord hit; 

	if(world->hit(r, 0.0f, MAXFLOAT, hit)) {
		return 0.5f * vec3(hit.normal.x() + 1.0f, hit.normal.y() + 1.0f, hit.normal.z() + 1.0f);
	}
	else {
		vec3 vUnitDirection = UnitVector(r.direction());
		float t = 0.5f * (vUnitDirection.y() + 1.0f);
		return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
	}
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

	hitable *list[2];
	list[0] = new sphere(vec3(0.0f, 0.0f, -1.0f), 0.5f);
	list[1] = new sphere(vec3(0.0f, -100.5f, -1.0f), 100.0f);
	hitable *world = new HitableList(list, 2);

	for(int j = ny - 1; j >= 0; j--) {
		for(int i = 0; i < nx; i++) {
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);

			ray r(vOrigin, vLowerLeftCorner + u * vHorizontal + v * vVertical);

			vec3 p = r.PointAtParameter(2.0f);
			vec3 col = color(r, world);

			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);

			fileOutput << ir << " " << ig << " " << ib << "\n";
		}
	}

	fileOutput.close();

	return 0;
}