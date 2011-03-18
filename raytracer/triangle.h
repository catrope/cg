//  Authors:
//    Roan Kattouw
//    Jan Paul Posma

#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "object.h"

class Triangle : public Object
{
public:
	Triangle(Point pp1, Point pp2, Point pp3, const Vector &rot, double angle) :
		Object(rot, angle), p1(pp1), p2(pp2), p3(pp3)
	{ }
	Triangle(Point pp1, Point pp2, Point pp3) :
		Object(Vector(0, 0, 1), 0.0), p1(pp1), p2(pp2), p3(pp3)
	{ }

	virtual Hit intersect(const Ray &ray);
	virtual Point getRotationCenter();

	const Point p1, p2, p3;
};

#endif /* end of include guard: TRIANGLE_H */
