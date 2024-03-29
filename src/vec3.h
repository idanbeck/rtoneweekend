#ifndef VEC_3_H_
#define VEC_3_H_

#include <math.h>
#include <stdlib.h>
#include <iostream>

#include "drand48.h"

class vec3 {
public:
	vec3() {}

	vec3(float val) {
		e[0] = val;
		e[1] = val;
		e[2] = val;
	}

	vec3(float e0, float e1, float e2) {
		e[0] = e0;
		e[1] = e1;
		e[2] = e2;
	}

	inline float x() const { return e[0]; };
	inline float y() const { return e[1]; };
	inline float z() const { return e[2]; };

	inline float r() const { return e[0]; };
	inline float g() const { return e[1]; };
	inline float b() const { return e[2]; };

	inline const vec3& operator+() const { return *this; }
	inline vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
	inline float operator[](int i) const { return e[i]; }
	inline float& operator[](int i) { return e[i]; }

	inline vec3& operator+=(const vec3 &v2);
	inline vec3& operator-=(const vec3 &v2);
	inline vec3& operator*=(const vec3 &v2);
	inline vec3& operator/=(const vec3 &v2);
	inline vec3& operator*=(const float t);
	inline vec3& operator/=(const float t);

	inline float length() const {
		return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
	}

	inline float squaredLength() const {
		return (e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
	}

	inline void MakeUnitVector();
	
	inline bool IsZero() const {
		return ((e[0] == 0.0f) && (e[1] == 0.0f) && (e[2] == 0.0f));
	}

public:
	float e[3];
};

inline std::istream& operator>>(std::istream &is, vec3 &t) {
	is >> t.e[0] >> t.e[1] >> t.e[2];
	return is;
}

inline std::ostream& operator<<(std::ostream &os, vec3 &t) {
	os << t.e[0] << " " << t.e[1] << " " << t.e[2];
	return os;
}

inline void vec3::MakeUnitVector() {
	float k = 1.0f / sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);

	e[0] *= k;
	e[1] *= k;
	e[2] *= k;
}

inline vec3 operator+(const vec3 &lhs, const vec3 &rhs) {
	return vec3(lhs.e[0] + rhs.e[0], 
				lhs.e[1] + rhs.e[1],
				lhs.e[2] + rhs.e[2]);
}

inline vec3 operator-(const vec3 &lhs, const vec3 &rhs) {
	return vec3(lhs.e[0] - rhs.e[0], 
				lhs.e[1] - rhs.e[1],
				lhs.e[2] - rhs.e[2]);
}

inline vec3 operator*(const vec3 &lhs, const vec3 &rhs) {
	return vec3(lhs.e[0] * rhs.e[0], 
				lhs.e[1] * rhs.e[1],
				lhs.e[2] * rhs.e[2]);
}

inline vec3 operator/(const vec3 &lhs, const vec3 &rhs) {
	return vec3(lhs.e[0] / rhs.e[0], 
				lhs.e[1] / rhs.e[1],
				lhs.e[2] / rhs.e[2]);
}

inline vec3 operator*(float t, const vec3 &rhs) {
	return vec3(t * rhs.e[0], 
				t * rhs.e[1],
				t * rhs.e[2]);
}

inline vec3 operator/(const vec3 &lhs, float t) {
	return vec3(lhs.e[0] / t, 
				lhs.e[1] / t,
				lhs.e[2] / t);
}

inline float dot(const vec3 &lhs, const vec3 &rhs) {
	return (	lhs.e[0] * rhs.e[0] +
			lhs.e[1] * rhs.e[1] +
			lhs.e[2] * rhs.e[2]);
}

inline vec3 cross(const vec3 &lhs, const vec3 &rhs) {
	return vec3(
		(lhs.e[1] * rhs.e[2] - lhs.e[2] * rhs.e[1]),
		(-(lhs.e[0] * rhs.e[2] - lhs.e[2] * rhs.e[0])),
		(lhs.e[0] * rhs.e[1] - lhs.e[1] * rhs.e[0])
	);
}

inline vec3& vec3::operator+=(const vec3 &v) {
	e[0] += v.e[0];
	e[1] += v.e[1];
	e[2] += v.e[2];
	return *this;
}

inline vec3& vec3::operator*=(const vec3 &v) {
	e[0] *= v.e[0];
	e[1] *= v.e[1];
	e[2] *= v.e[2];
	return *this;
}

inline vec3& vec3::operator/=(const vec3 &v) {
	e[0] /= v.e[0];
	e[1] /= v.e[1];
	e[2] /= v.e[2];
	return *this;
}

inline vec3& vec3::operator-=(const vec3 &v) {
	e[0] -= v.e[0];
	e[1] -= v.e[1];
	e[2] -= v.e[2];
	return *this;
}

inline vec3& vec3::operator*=(float t) {
	e[0] *= t;
	e[1] *= t;
	e[2] *= t;
	return *this;
}

inline vec3& vec3::operator/=(float t) {
	float k = 1.0f / t;

	e[0] *= k;
	e[1] *= k;
	e[2] *= k;

	return *this;
}

inline vec3 UnitVector(vec3 v) {
	return v / v.length();
}

vec3 RandomVectorInUnitSphere() {
	vec3 p;
	
	do {
		p = 2.0f * vec3(drand48(), drand48(), drand48()) - vec3(1.0f, 1.0f, 1.0f);
	} while(p.squaredLength() >= 1.0f);

	return p;
}

inline vec3 reflect(const vec3& v, const vec3& n) {
	return (v - 2.0f*dot(v, n)*n);
}

inline bool refract(const vec3& v, const vec3& n, float NIOverNT, vec3 &vRefract) {
	vec3 vUnit = UnitVector(v);
	float dt = dot(vUnit, n);
	float discriminant = 1.0f - NIOverNT * NIOverNT * (1.0f - dt*dt);
	if(discriminant > 0) {
		vRefract = NIOverNT * (vUnit - n * dt) - n * sqrt(discriminant);
		return true;
	}
	else {
		return false;
	}
}

#endif // ! VEC_3_H_
