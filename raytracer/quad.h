//  Authors:
//    Roan Kattouw
//    Jan Paul Posma

#ifndef QUAD_H
#define QUAD_H

#include "object.h"

class Quad : public Object
{
public:
	Quad(Point pp1, Point pp2, Point pp3, Point pp4) :
		Object(), p1(pp1), p2(pp2), p3(pp3), p4(pp4)
	{ }

	virtual Hit intersect(const Ray &ray);

	const Point p1, p2, p3, p4;
};

#endif /* end of include guard: QUAD_H */
