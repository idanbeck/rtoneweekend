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

hitable *randomScene() {
	int n = 500;
	hitable **ppList = new hitable*[n + 1];
	ppList[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5f, 0.5f, 0.5f)));

	int i = 1;
	
	/*
	for(int a = -11; a < 11; a++) {
		for(int b = -11; b < 11; b++) {
			float chooseMat = drand48();
			vec3 center(a + 0.9f * drand48(), 0.2f, b + 0.9f * drand48());
			if((center - vec3(4, 0.2, 0)).length() > 0.9f) {
				if(chooseMat < 0.8f) { 			// diffuse
					ppList[i++] = new sphere(center, 0.2f, new lambertian(vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48())));
				}
				else if(chooseMat < 0.95f) {	// metal
					ppList[i++] = new sphere(center, 0.2f, 
						new metal(vec3(0.5*(1 + drand48()), 0.5*(1 + drand48()), 0.5*(1 + drand48())), 0.5*drand48()));
				}
				else {		// glass
					ppList[i++] = new sphere(center, 0.2f, new dialectric(1.5f));
				}
			}
		}
	}
	 */

	ppList[i++] = new sphere(vec3(0, 1, 0), 1.0f, new dialectric(1.5f));
	ppList[i++] = new sphere(vec3(-4, 1, 0), 1.0f, new lambertian(vec3(0.4, 0.2, 0.1)));
	ppList[i++] = new sphere(vec3(4, 1, 0), 1.0f, new metal(vec3(0.7, 0.6, 0.5), 0.0));

	return new HitableList(ppList, i);
}

int main(int argc, char *argv[]) {
	int scale = 1;
	int nx = 640 * scale;
	int ny = 480 * scale;
	int ns = 100;
	float pctComplete = 0.0f;

	std::ofstream fileOutput;
  	fileOutput.open("out.ppm");

	fileOutput << "P3\n" << nx << " " << ny << "\n255\n";

	hitable *world = randomScene();

	vec3 lookFrom(10, 1.5, 3);
	vec3 lookAt(0, 1, 0);
	float focusDistance = (lookFrom - lookAt).length();
	float aperture = 0.1f;

	camera cam(
		lookFrom,			// origin
		lookAt,			// look at point
		vec3(0, 1, 0),			// up vector
		45, 					// FOV
		float(nx) / float(ny),	// aspect ratio
		aperture,	
		focusDistance
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
				//vec3 p = r.PointAtParameter(2.0f);
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
