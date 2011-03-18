//  Authors:
//    Roan Kattouw
//    Jan Paul Posma

#ifndef QUAD_H
#define QUAD_H

#include "object.h"

class Quad : public Object
{
public:
	Quad(Point pp1, Point pp2, Point pp3, Point pp4, const Vector &rot, double angle) :
		Object(rot, angle), p1(pp1), p2(pp2), p3(pp3), p4(pp4)
	{
		// Use intermediate variables for p1 .. p4 because the
		// original values need to be used to compute the rotation center
		Point newp1, newp2, newp3, newp4;
		newp1 = rotate(p1);
		newp2 = rotate(p2);
		newp3 = rotate(p3);
		newp4 = rotate(p4);
		
		p1 = newp1;
		p2 = newp2;
		p3 = newp3;
		p4 = newp4;
	}
	
	virtual Hit intersect(const Ray &ray);
	virtual Point getRotationCenter();

	Point p1, p2, p3, p4;
};

#endif /* end of include guard: QUAD_H */
