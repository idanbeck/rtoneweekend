#ifndef MATERIAL_H_
#define MATERIAL_H_

class material {
public:
	virtual bool scatter(const ray& rIn, const HitRecord &hit, vec3& attenuation, ray& rScattered) const = 0;
};

#endif // ! MATERIAL_H_