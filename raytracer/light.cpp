//
//  Framework for a raytracer
//  File: light.cpp
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Authors:
//    Maarten Everts
//    Jasper van de Gronde
//    Roan Kattouw
//    Jan Paul Posma
//
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//
#include "light.h"

Light::Light(Point pos, Color c, double r) : Object(Vector(0, 0, 1), 0.0), position(pos), color(c)
{
	boundingSphere = new Sphere(pos, r);
	material = new Material();
	material->color = c;
	material->light = true;
}

Hit Light::intersect(const Ray &ray, bool closest, double maxT)
{
	return Hit::NO_HIT();
	Hit hit = boundingSphere->intersect(ray, closest, maxT);
	hit.makeObj(this);
	return hit;
}
