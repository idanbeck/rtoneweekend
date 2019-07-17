#ifndef PERLIN_H_
#define PERLIN_H_

#include <math.h>
#include "vec3.h"

inline float PerlinInterpolation(vec3 cv[2][2][2], float u, float v, float w) {
	float uu = u*u*(3.0f - 2.0f * u);
	float vv = v*v*(3.0f - 2.0f * v);
	float ww = w*w*(3.0f - 2.0f * w);
	
	float accumulator = 0.0f;
	
	for(int i = 0; i < 2; i++)
		for(int j = 0; j < 2; j++)
			for(int k = 0; k < 2; k++) {
				vec3 vWeight(u - i, v - j, w - k);
				//float val = ((dot(cv[i][j][k], vWeight) + 1.0f) / 2.0f);
				float val = dot(cv[i][j][k], vWeight);
				
				
				accumulator += 	((i * uu) + (1.0f - i)*(1.0f - uu)) *
								((j * vv) + (1.0f - j)*(1.0f - vv)) *
								((k * ww) + (1.0f - k)*(1.0f - ww)) * val;
			
			}
	
	return accumulator;
}

inline float PerlinInterpolation(float cf[2][2][2], float u, float v, float w) {
	float uu = u*u*(3.0f - 2.0f * u);
	float vv = v*v*(3.0f - 2.0f * v);
	float ww = w*w*(3.0f - 2.0f * w);
	
	float accumulator = 0.0f;
	
	for(int i = 0; i < 2; i++)
		for(int j = 0; j < 2; j++)
			for(int k = 0; k < 2; k++) {
				accumulator +=  ((i * uu) + (1 - i)*(1 - uu)) *
								((j * vv) + (1 - j)*(1 - vv)) *
								((k * ww) + (1 - k)*(1 - ww)) * cf[i][j][k];
				
			}
	
	return accumulator;
}

class perlin {
public:
	perlin() {}
	
	float noise(const vec3& p) const {
		float u = p.e[0] - floor(p.e[0]);
		float v = p.e[1] - floor(p.e[1]);
		float w = p.e[2] - floor(p.e[2]);
		
		//float cf[2][2][2];
		vec3 cv[2][2][2];
		
		for(int di = 0; di < 2; di++)
			for(int dj = 0; dj < 2; dj++)
				for(int dk = 0; dk < 2; dk++) {
					cv[di][dj][dk] = m_ranvec[m_permX[int(floor(p.e[0]) + di) & 255] ^ m_permY[int(floor(p.e[1]) + dj) & 255] ^ m_permZ[int(floor(p.e[2]) + dk) & 255]];
					//cf[di][dj][dk] = m_ranfloat[m_permX[int(floor(p.e[0]) + di) & 255] ^ m_permY[int(floor(p.e[1]) + dj) & 255] ^ m_permZ[int(floor(p.e[2]) + dk) & 255]];
				}
		
		//return PerlinInterpolation(cf, u, v, w);
		return PerlinInterpolation(cv, u, v, w);
	}
	
	float turbulence(const vec3& p, int depth = 7) const {
		float accumulator = 0.0f;
		vec3 vTemp = p;
		float weight = 1.0f;
		
		for (int i = 0; i < depth; i++) {
			accumulator += (weight * noise(vTemp));
			weight *= 0.5f;
			vTemp *= 2.0f;
		}
		
		return fabs(accumulator);
	}
	
	static vec3 *m_ranvec;
	static float *m_ranfloat;
	static int *m_permX;
	static int *m_permY;
	static int *m_permZ;
};

static float *GeneratePerlinFloat() {
	float *p = new float[256];
	
	for (int i = 0; i < 256; ++i) {
		//p[i] = UnitVector(vec3(-1.0f + 2.0f * drand48(), -1.0f + 2.0f * drand48(), -1.0f + 2.0f * drand48()));
		p[i] = drand48();
	}
	
	return p;
}

static vec3* GeneratePerlinVector() {
	vec3 *p = new vec3[256];
	
	for (int i = 0; i < 256; ++i) {
		p[i] = UnitVector(vec3(-1.0f + 2.0f * drand48(), -1.0f + 2.0f * drand48(), -1.0f + 2.0f * drand48()));
	}
	
	return p;
}

void permute(int *p, int n) {
	for(int i = n - 1; i > 0; i--) {
		int target = int(drand48() * (i + 1));
		std::swap(p[i], p[target]);
	}
	return;
}

static int* GeneratePerlinPerm() {
	int *p = new int[256];
	
	for(int i = 0; i < 256; i++) {
		p[i] = i;
	}
	
	permute(p, 256);
	
	return p;
}

vec3  *perlin::m_ranvec = GeneratePerlinVector();
float  *perlin::m_ranfloat = GeneratePerlinFloat();
int *perlin::m_permX = GeneratePerlinPerm();
int *perlin::m_permY = GeneratePerlinPerm();
int *perlin::m_permZ = GeneratePerlinPerm();


#endif // ! PERLIN_H_
