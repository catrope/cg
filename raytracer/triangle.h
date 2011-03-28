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
	{
		// Use intermediate variables for p1 .. p3 because the
		// original values need to be used to compute the rotation center
		Point newp1, newp2, newp3;
		newp1 = rotate(p1);
		newp2 = rotate(p2);
		newp3 = rotate(p3);
		
		p1 = newp1;
		p2 = newp2;
		p3 = newp3;
	}
	Triangle(Point pp1, Point pp2, Point pp3) :
		Object(Vector(0, 0, 1), 0.0), p1(pp1), p2(pp2), p3(pp3)
	{ }

	virtual Hit intersect(const Ray &ray, bool closest, double maxT);
	virtual Point getRotationCenter();

	Point p1, p2, p3;
};

#endif /* end of include guard: TRIANGLE_H */
