#ifndef BVH_NODE_H_
#define BVH_NODE_H_

#include "hitable.h"

class BVHNode : public hitable {
public:
	BVHNode() {}
	BVHNode(hitable **ppList, int ppList_n, float time0, float time1);
	virtual bool hit(const ray& r, float tmin, float tmax, HitRecord& hit) const override;
	virtual bool BoundingBox(float t0, float t1, aabb& box) const override;
	
	hitable *m_pLeft;
	hitable *m_pRight;
	
	aabb m_box;
};

bool BVHNode::BoundingBox(float t0, float t1, aabb& box) const {
	box = m_box;
	return true;
}

BVHNode::BVHNode(hitable **ppList, int ppList_n, float time0, float time1) {
	int acis = int(3 * drand48());
	
	switch(axis) {
		case 0: {
			qsort(ppList, pList_n, sizeof(hitable*), aabb::BoxXCompare);
		} break;
			
		case 1: {
			qsort(ppList, pList_n, sizeof(hitable*), aabb::BoxYCompare);
		} break;
			
		case 2: {
			qsort(ppList, pList_n, sizeof(hitable*), aabb::BoxZCompare);
		} break;
	}
	
	if(n == 1) {
		m_pLeft = m_pRight = ppList[0];
	}
	else if(n == 2) {
		m_pLeft = ppList[0];
		m_pRight = ppList[1];
	}
	else {
		m_pLeft = new BVHNode(ppList, ppList_n / 2, time0, time1);
		m_pRight = new BVHNode(ppList + ppList_n / 2, time0, time1);
	}
	
	aabb boxLeft, boxRight;
	
	if(m_pLeft->BoundingBox(time0, time1, boxLeft) == false || m_pRight->BoundingBox(time0, time1, boxRight) == false) {
		std::cerr << "no bounding box in BVHNode constructor\n";
	}
	
	m_box = aabb::SurroundingBox(boxLeft, boxRight);
}

bool BVHNode::hit(const ray& r, float tmin, float tmax, HitRecord& hit) const {
	if(m_box.hit(r, tmin, tmax)) {
		HitRecord leftHit, rightHit;
		
		bool fHitLeft = m_pLeft->hit(r, tmin, tmax, leftHit);
		bool fHitRight = m_pRight->hit(r, tmin, tmax, rightHit);
		
		if(fHitLeft && fHitRight) {
			if(leftHit.t < rightHit.t)
				hit = leftHit;
			else
				hit = rightHit;
			
			return true;
		}
		else if(fHitLeft) {
			hit = leftHit;
			return true;
		}
		else if(fHitRight) {
			hit = rightHit;
			return true;
		}
	}
	
	return false;
}

#endif // ! BVH_NODE_H_
