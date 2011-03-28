//  Authors:
//    Roan Kattouw
//    Jan Paul Posma

#include "triangle.h"
#include "quad.h"
#include <iostream>
#include <math.h>

Hit Quad::intersect(const Ray &ray, bool closest, double maxT)
{
	Hit h1 = Triangle(p1, p2, p3).intersect(ray, closest, maxT);
	
	if (h1.hasHit() && !closest) return Hit(h1.t, h1.N, this);
	
	Hit h2 = Triangle(p1, p3, p4).intersect(ray, closest, maxT);
	double inf = std::numeric_limits<double>::infinity();
	
	if (!(h1.hasHit() || h2.hasHit())) return Hit::NO_HIT();
	
	Hit h = ( (h1.t < inf) ? h1 : h2);
	return Hit(h.t, h.N, this);
}

Point Quad::getRotationCenter()
{
	return (p1 + p2 + p3 + p4)/4;
}