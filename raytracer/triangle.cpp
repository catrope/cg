//  Authors:
//    Roan Kattouw
//    Jan Paul Posma

#include "triangle.h"
#include <iostream>
#include <math.h>

Hit Triangle::intersect(const Ray &ray, bool closest, double maxT)
{
	// algorithm of pages 206-208
	double a, b, c, d, e, f, g, h, i, j, k, l, M, beta, gamma, t;
	a = p1.x - p2.x; b = p1.y - p2.y; c = p1.z - p2.z;
	d = p1.x - p3.x; e = p1.y - p3.y; f = p1.z - p3.z;
	g = ray.D.x; h = ray.D.y; i = ray.D.z;
	j = p1.x - ray.O.x; k = p1.y - ray.O.y; l = p1.z - ray.O.z;
	
	M = a*(e*i - h*f) + b*(g*f - d*i) + c*(d*h - e*g);
	
	t = -(f*(a*k - j*b) + e*(j*c - a*l) + d*(b*l - k*c))/M;
	
	if (t < 0) return Hit::NO_HIT();
	
	gamma = (i*(a*k - j*b) + h*(j*c - a*l) + g*(b*l - k*c))/M;
	
	if (gamma < 0 || gamma > 1) return Hit::NO_HIT();
	
	beta = (j*(e*i - h*f) + k*(g*f - d*i) + l*(d*h - e*g))/M;
	
	if (beta < 0 || beta > (1 - gamma)) return Hit::NO_HIT();
	
	Vector N = ((p2 - p1).cross(p3 - p1)).normalized();
	return Hit(t, N, this);
}

Point Triangle::getRotationCenter()
{
	// Use the centroid of the triangle, which is also the center of mass
	// (assuming the triangle is of uniform density) and the average of
	// the vertices
	return (p1 + p2 + p3)/3;
}

void Triangle::getTexCoords(const Point &p, double &u, double &v)
{
	// Use the dot product with vectors p2-p1 and p3-p1
	Vector d1 = p2-p1;
	Vector d2 = p3-p1;
	
	u = d1.dot((p-p1))/d1.length_2();
	v = d2.dot((p-p1))/d2.length_2();
}