#include <iostream>
#include <iostream>
#include <fstream>

#include "vec3.h"
#include "ray.h"
#include "float.h"
#include "HitableList.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

#include "metal.h"
#include "lambertian.h"
#include "dialectric.h"

vec3 color(const ray& r, hitable *world, int depth) {
	HitRecord hit; 

	if(world->hit(r, 0.001f, MAXFLOAT, hit)) {
		//return 0.5f * vec3(hit.normal.x() + 1.0f, hit.normal.y() + 1.0f, hit.normal.z() + 1.0f);
		//vec3 target = hit.p + hit.normal + randomInUnitSphere();
		//return 0.5f * color(ray(hit.p, target - hit.p), world);
		ray rScattered;
		vec3 attenuation;
		if(depth < 50 && hit.pMaterial->scatter(r, hit, attenuation, rScattered)) {
			return attenuation * color(rScattered, world, depth + 1);
		}
		else {
			return vec3(0.0f);
		}
	}
	else {
		vec3 vUnitDirection = UnitVector(r.direction());
		float t = 0.5f * (vUnitDirection.y() + 1.0f);
		return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
	}
}

int main(int argc, char *argv[]) {
	int scale = 1;
	int nx = 200 * scale;
	int ny = 100 * scale;
	int ns = 100;
	float pctComplete = 0.0f;

	std::ofstream fileOutput;
  	fileOutput.open("out.ppm");

	fileOutput << "P3\n" << nx << " " << ny << "\n255\n";

	float R = cos(M_PI/4.0f);

	hitable *list[] = {
		new sphere(vec3(0.0f, 0.0f, -1.0f), 0.5f, new lambertian(vec3(0.1f, 0.2f, 0.5f))),
		new sphere(vec3(0.0f, -100.5f, -1.0f), 100.0f, new lambertian(vec3(0.8f, 0.8f, 0.0f))),
		new sphere(vec3(1.0f, 0.0f, -1.0f), 0.5f, new metal(vec3(0.8f, 0.6f, 0.2f), 0.3f)),
		new sphere(vec3(-1.0f, 0.0f, -1.0f), 0.5f, new dialectric(1.5f)),
		new sphere(vec3(-1.0f, 0.0f, -1.0f), -0.45f, new dialectric(1.5f)),
		// new sphere(vec3(-R, 0.0f, -1.0f), R, new lambertian(vec3(0, 0, 1))),
		// new sphere(vec3(R, 0.0f, -1.0f), R, new lambertian(vec3(1, 0, 0))),
	};

	hitable *world = new HitableList(list, sizeof(list) / sizeof(list[0]));
	camera cam(
		vec3(-2, 2, 1),			// origin
		vec3(0, 0, -1),			// look at point
		vec3(0, 1, 0),			// up vector
		60, 					// FOV
		float(nx) / float(ny)	// aspect ratio
	);

	// For read out
	std::cout.precision(2);

	for(int j = ny - 1; j >= 0; j--) {
		for(int i = 0; i < nx; i++) {
			vec3 col(0.0f, 0.0f, 0.0f);
			for(int s = 0; s < ns; s++) {
				float u = float(i + drand48()) / float(nx);
				float v = float(j + drand48()) / float(ny);

				ray r = cam.getRay(u, v);
				vec3 p = r.PointAtParameter(2.0f);
				col += color(r, world, 0);
				
			}
			col /= ns;
			// Gamma correct
			col = vec3(sqrt(col.r()), sqrt(col.g()), sqrt(col.b()));
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);
			fileOutput << ir << " " << ig << " " << ib << "\n";
			pctComplete = float((ny - 1 - j) * nx + i) / float(ny * nx);
			
			
			std::cout << "\r" << float(pctComplete * 100.0f);
		}
	}

	fileOutput.close();

	return 0;
}