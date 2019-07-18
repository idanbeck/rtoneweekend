#include <iostream>
#include <iostream>
#include <fstream>
#include <chrono>
#include <cstdlib>
#include <limits>

#include "vec3.h"
#include "ray.h"
#include "HitableList.h"
#include "BVHNode.h"
#include "sphere.h"
#include "rect.h"
#include "camera.h"
#include "material.h"

#include "metal.h"
#include "lambertian.h"
#include "dialectric.h"

#include "ConstantTexture.h"
#include "CheckerTexture.h"
#include "NoiseTexture.h"
#include "DiffuseLight.h"

#include "drand48.h"

vec3 color(const ray& r, hitable *world, int depth) {
	HitRecord hit; 

	if(world->hit(r, 0.001f, std::numeric_limits<float>::max(), hit)) {
		//return 0.5f * vec3(hit.normal.x() + 1.0f, hit.normal.y() + 1.0f, hit.normal.z() + 1.0f);
		//vec3 target = hit.p + hit.normal + randomInUnitSphere();
		//return 0.5f * color(ray(hit.p, target - hit.p), world);
		ray rScattered;
		vec3 attenuation;
		vec3 emitted = hit.pMaterial->emitted(hit.u, hit.v, hit.p);

		if(depth < 50 && hit.pMaterial->scatter(r, hit, attenuation, rScattered)) {
			return emitted + attenuation * color(rScattered, world, depth + 1);
		}
		else {
			return emitted;
		}
	}
	else {
		// vec3 vUnitDirection = UnitVector(r.direction());
		// float t = 0.5f * (vUnitDirection.y() + 1.0f);
		// return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
		return vec3(0.0f, 0.0f, 0.0f);
	}
}

hitable *CornellBox() {
	hitable **ppList = new hitable*[6];
	int i = 0;
	material *pRed = new lambertian(new ConstantTexture(vec3(0.65f, 0.05f, 0.05f)));
	material *pWhite = new lambertian(new ConstantTexture(vec3(0.73f, 0.73f, 0.73f)));
	material *pGreen = new lambertian(new ConstantTexture(vec3(0.12f, 0.45f, 0.15f)));
	material *pLight = new DiffuseLight(new ConstantTexture(vec3(15.0f, 15.0f, 15.0f)));

	ppList[i++] = new FlipNormals(new YZRect(0, 555, 0, 555, 555, pGreen));
	ppList[i++] = new YZRect(0, 555, 0, 555, 0, pRed);
	ppList[i++] = new XZRect(213, 343, 227, 332, 554, pLight);
	ppList[i++] = new FlipNormals(new XZRect(0, 555, 0, 555, 555, pWhite));
	ppList[i++] = new XZRect(0, 555, 0, 555, 0, pWhite);
	ppList[i++] = new FlipNormals(new XYRect(0, 555, 0, 555, 555, pWhite));

	return new HitableList(ppList, i);
}

hitable *TwoPerlinSpheres() {
	texture *pPerlin = new NoiseTexture(5.0f);
	
	int pList_n = 2;
	hitable **ppList = new hitable*[pList_n];
	ppList[0] = new sphere(vec3(0.0f, -1000.0f, 0.0f), 1000.0f, new lambertian(pPerlin));
	ppList[1] = new sphere(vec3(0.0f, 2.0f, 0.0f), 2.0f, new lambertian(pPerlin));
	
	//ppList[1] = new sphere(vec3(0.0f, 2.0f, 0.0f), 2.0f, new DiffuseLight(pPerlin));
	
	return new HitableList(ppList, 2);
}

hitable *SimpleLight() {
	texture *pPerlin = new NoiseTexture(5.0f);
	
	int pList_n = 4;
	hitable **ppList = new hitable*[pList_n];

	int count = 0;
	ppList[count++] = new sphere(vec3(0.0f, -1000.0f, 0.0f), 1000.0f, new lambertian(pPerlin));
	ppList[count++] = new sphere(vec3(0.0f, 2.0f, 0.0f), 2.0f, new lambertian(pPerlin));
	ppList[count++] = new sphere(vec3(0.0f, 7.0f, 0.0f), 2.0f, new DiffuseLight( new ConstantTexture(vec3(4.0f, 4.0f, 4.0f))));
	ppList[count++] = new XYRect(3, 5, 1, 3, -2.0f, new DiffuseLight( new ConstantTexture(vec3(4.0f, 4.0f, 4.0f))));
	
	//ppList[1] = new sphere(vec3(0.0f, 2.0f, 0.0f), 2.0f, new DiffuseLight(pPerlin));
	
	return new HitableList(ppList, count);
}

hitable *randomScene() {
	int n = 500;
	hitable **ppList = new hitable*[n + 1];

	texture *pTexture = new CheckerTexture(new ConstantTexture(vec3(0.2f, 0.3f, 0.1f)), new ConstantTexture(vec3(0.9f, 0.9f, 0.9f)));
	ppList[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(pTexture));

	int i = 1;
	int range = 11;
	
	///*
	for(int a = -range; a < range; a++) {
		for(int b = -range; b < range; b++) {
			float chooseMat = drand48();
			vec3 center(a + 0.9f * drand48(), 0.2f, b + 0.9f * drand48());
			if((center - vec3(4, 0.2, 0)).length() > 0.9f) {
				if(chooseMat < 0.8f) { 			// diffuse
					pTexture = new ConstantTexture(vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48()));
					ppList[i++] = new sphere(center, 0.2f, new lambertian(pTexture));
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
	//*/
	
	ppList[i++] = new sphere(vec3(0, 1, 0), 1.0f, new dialectric(1.5f));

	
	pTexture = new ConstantTexture(vec3(0.3f, 0.2f, 0.1f));
	ppList[i++] = new sphere(vec3(-4, 1, 0), 1.0f, new lambertian(pTexture));
	//dynamic_cast<sphere*>(ppList[i - 1])->SetVelocity(vec3(0.0f, 1.0f, 0.0f));
	
	ppList[i++] = new sphere(vec3(4, 1, 0), 1.0f, new metal(vec3(0.7, 0.6, 0.5), 0.0));

	//return new HitableList(ppList, i);
	return new BVHNode(ppList, i, 0.0f, 0.0f);
}

int main(int argc, char *argv[]) {
	float scale = 1.0f;
	int nx = 640 * scale;
	int ny = 480 * scale;
	int ns = 10000;
	float pctComplete = 0.0f;
	
	//InitializeRand();

	auto timeStart = std::chrono::high_resolution_clock::now();

	std::cout << "starting...";

	std::ofstream fileOutput;
  	fileOutput.open("out.ppm");

	fileOutput << "P3\n" << nx << " " << ny << "\n255\n";

	//hitable *world = randomScene();
	//hitable *world = TwoPerlinSpheres();
	//hitable *world = SimpleLight();
	hitable *world = CornellBox();

	vec3 lookFrom(278, 278, -800);
	vec3 lookAt(278, 278, 0);
	//float focusDistance = (lookFrom - lookAt).length();
	float focusDistance = 10.0f;
	//float aperture = 0.1f;
	float aperture = 0.0f;
	float FOV = 40.0f;

	camera cam(
		lookFrom,			// origin
		lookAt,			// look at point
		vec3(0, 1.0f, 0),			// up vector
		FOV, 					// FOV
		float(nx) / float(ny),	// aspect ratio
		aperture,	
		focusDistance,
		0.0f,
		1.0f
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
	
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - timeStart);
	std::cout << "Total time: " << duration.count() << " ms\n";
	
	fileOutput.close();

	return 0;
}
