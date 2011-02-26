//  Authors:
//    Roan Kattouw
//    Jan Paul Posma

#include "triangle.h"
#include "quad.h"
#include <iostream>
#include <math.h>

Hit Quad::intersect(const Ray &ray)
{
	Hit h1 = Triangle(p1, p2, p3).intersect(ray);
	Hit h2 = Triangle(p1, p3, p4).intersect(ray);
	double inf = std::numeric_limits<double>::infinity();
	
	if (!(h1.t < inf || h2.t < inf)) return Hit::NO_HIT();
	
	return ( (h1.t < inf) ? h1 : h2);
}