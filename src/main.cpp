#include <iostream>
#include <iostream>
#include <fstream>

#include "vec3.h"

int main(int argc, char *argv[]) {
	int nx = 640;
	int ny = 480;

	std::ofstream fileOutput;
  	fileOutput.open("out.ppm");

	fileOutput << "P3\n" << nx << " " << ny << "\n255\n";

	for(int j = ny - 1; j >= 0; j--) {
		for(int i = 0; i < nx; i++) {
			float r = float(i) / float(nx);
			float g = float(j) / float(ny);
			float b = 0.2f;

			int ir = int(255.99*r);
			int ig = int(255.99*g);
			int ib = int(255.99*b);

			fileOutput << ir << " " << ig << " " << ib << "\n";
		}
	}

	fileOutput.close();

	return 0;
}