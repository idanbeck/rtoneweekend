#ifndef NOISE_TEXTURE_H_
#define NOISE_TEXTURE_H_

#include "perlin.h"
#include "texture.h"

class NoiseTexture : public texture {
public:
	NoiseTexture() {}
	NoiseTexture(float scale) :
		m_scale(scale)
	{
		m_noise = perlin();
	}
	
	virtual vec3 value(float u, float v, const vec3& p) const override {
		//return vec3(1.0f, 1.0f, 1.0f) * fabs(m_noise.noise(m_scale * p));
		
		//return vec3(1.0f, 1.0f, 1.0f) * 0.5f * m_noise.turbulence(m_scale * p);
		//return vec3(1.0f, 1.0f, 1.0f)  * m_noise.turbulence(m_scale * p);
		
		return vec3(1.0f, 1.0f, 1.0f) * 0.5f * fabs(sin(m_scale * p.z() + 10.0f * m_noise.turbulence(p)));
	}
	
	perlin m_noise;
	float m_scale;
};

#endif // NOISE_TEXTURE_H_
