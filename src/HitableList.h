#ifndef HITABLE_LIST_H_
#define HITABLE_LIST_H_

#include "hitable.h"

class HitableList : public hitable {
public:
	HitableList() {}
	HitableList(hitable **l, int n) {
		list = l;
		list_n = n;
	}

	virtual bool hit(const ray &r, float tMin, float tMax, HitRecord &rec) const override;
	virtual bool BoundingBox(float t0, float t1, aabb& box) const override;

	hitable **list = nullptr;
	int list_n = 0;
};

bool HitableList::BoundingBox(float t0, float t1, aabb &box) const {
	if(list_n < 1)
		return false;
	
	aabb tempBox;
	bool fFirst = list[0]->BoundingBox(t0, t1, tempBox);
	if(fFirst == false) {
		return false;
	}
	else {
		box = tempBox;
	}
	
	for(int i = 0; i < list_n; i++) {
		if(list[i]->BoundingBox(t0, t1, tempBox)) {
			box = aabb::SurroundingBox(box, tempBox);
		}
		else {
			return false;
		}
	}
	
	return true;
}

bool HitableList::hit(const ray &r, float tMin, float tMax, HitRecord &rec) const {
	HitRecord tempRecord;
	bool fHit = false;

	double tClosest = tMax;

	for(int i = 0; i < list_n; i++) {
		if(list[i]->hit(r, tMin, tClosest, tempRecord)) {
			fHit = true;
			tClosest = tempRecord.t;
			rec = tempRecord;
		}
	}

	return fHit;
}

#endif // ! HITABLE_LIST_H_
