//
//  Framework for a raytracer
//  File: sphere.cpp
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Authors:
//    Maarten Everts
//    Jasper van de Gronde
//
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//
//    Roan Kattouw
//    Jan Paul Posma

#include "csg.h"
#include <iostream>
#include <math.h>

/************************** Sphere **********************************/

Hit Csg::intersect(const Ray &ray, bool closest, double maxT)
{
	Hit hit = Hit::NO_HIT();
	switch (op)
	{
		/*
		case opIntersect:
			hit = isect(ray);
			break;
		case opDifference:
			hit = diff(ray);
			break;
		case opUnion:*/
		default:
			hit = closestHit(ray);
			break;
	}
	hit.makeObj(this);
	return hit;
}

Hit Csg::closestHit(const Ray &ray)
{
	static const double inf = std::numeric_limits<double>::infinity();
	Hit h1 = o1->intersect(ray, true, inf);
	Hit h2 = o2->intersect(ray, true, inf);
	
	if (!(h1.hasHit() || h2.hasHit())) return Hit::NO_HIT();
	
	return ( (h1.t < h2.t) ? h1 : h2);
}

Hit Csg::isect(Ray ray)
{
	bool in1 = false, in2 = false;
	double t = 0.0;
	
	Hit hit = Hit::NO_HIT();
	while(!(in1 && in2))
	{
		hit = closestHit(ray);
		if (!hit.hasHit()) return hit;
		
		if (hit.obj == o1) in1 = !in1;
		else in2 = !in2;
		
		hit.t += 0.01;
		t += hit.t;
		ray = Ray(ray.O + hit.t*ray.D, ray.D);
	}
	
	hit.t = t-0.01;
	return hit;
}

Hit Csg::diff(Ray ray)
{
	bool in1 = false, in2 = false;
	double t = 0.0;
	
	Hit hit = Hit::NO_HIT();
	while(!(in1 && !in2))
	{
		hit = closestHit(ray);
		if (!hit.hasHit()) return hit;
		
		if (hit.obj == o1) in1 = !in1;
		else in2 = !in2;
		
		hit.t += 0.01;
		t += hit.t;
		ray = Ray(ray.O + hit.t*ray.D, ray.D);
	}
	
	hit.t = t-0.01;
	return hit;
}
