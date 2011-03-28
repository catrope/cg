//
//  Framework for a raytracer
//  File: sphere.h
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

#ifndef SPHERE_H_115209AE
#define SPHERE_H_115209AE

#include "object.h"

class Sphere : public Object
{
public:
	Sphere(Point position, double r, const Vector &rot, double angle) : Object(rot, angle), position(position), r(r) { }
	Sphere(Point position, double r) : Object(Vector(0, 0, 1), 0.0), position(position), r(r) { }

	virtual Hit intersect(const Ray &ray, bool closest, double maxT);
	virtual Point getRotationCenter() { return position; }
	virtual void getTexCoords(const Point &p, double &x, double &y);
	virtual Point getPointFromTexCoords(double u, double v);

	const Point position;
	const double r;
	
	double getRadius() { return r; };
};

#endif /* end of include guard: SPHERE_H_115209AE */
