//  Authors:
//    Roan Kattouw
//    Jan Paul Posma

#include "triangle.h"
#include "quad.h"
#include <iostream>
#include <math.h>

Hit Quad::intersect(const Ray &ray, bool closest, double maxT)
{
	// simply pick the closest hit
	static const double inf = std::numeric_limits<double>::infinity();
	Hit h1 = t1->intersect(ray, true, inf);
	Hit h2 = t2->intersect(ray, true, inf);
	Hit h = ( (h1.t < h2.t) ? h1 : h2);
	
	h.makeObj(this);
	return h;
}

Point Quad::getRotationCenter()
{
	return (p1 + p2 + p3 + p4)/4;
}