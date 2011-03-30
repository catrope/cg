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

#include "cylinder.h"
#include <iostream>
#include <math.h>

/************************** Sphere **********************************/

Hit Cylinder::intersect(const Ray &ray, bool closest, double maxT)
{
	/*
	 * Line-cylinder intersection derivation (own work).
	 * Cylinder is defined by a line between two points (A, B) and a
	 * radius (r). For the calculation we assume that AB is on the x axis
	 * and starts in the origin, i.e. A=(0,0,0) and B=(L, 0, 0).
	 *
	 * A point P is on the cylinder if Py^2 + Pz^2 = r^2 and 0 <= Px <= L
	 * We're intersecting the cylinder with an incoming ray R: P=O+t*D
	 *
	 * (Oy + t*Dy)^2 + (Oz + t*Dz)^2 = r^2
	 * Oy^2 + t^2*Dy^2 + 2*Oy*t*Dy + Oz^2 + t^2*Dz^2 + 2*Oz*t*Dz = r^2
	 * (Dy^2 + Dz^2)*t^2 + (2*Oy*Dy + 2*Oz*Dz)*t + (Oy^2 + Oz^2 - r^2) = 0
	 * Solve for t with the quadratic formula, and check that the x value
	 * of O+t*D is between 0 and L (=|B-A|).
	 */
	
	// The intersection computation takes place in a translated and rotated
	// system where A=(0,0,0) and B=(L,0,0)
	// Transform the ray to this system
	Point O = rotToX*(ray.O - A);
	Vector D = (rotToX*ray.D).normalized();
	
	// Quadratic formula
	double a = D.y*D.y + D.z*D.z;
	double b = 2*O.y*D.y + 2*O.z*D.z;
	double c = O.y*O.y + O.z*O.z - r*r;
	double d = b*b - 4*a*c;
	if (d < 0)
		// No solutions
		return Hit::NO_HIT();
	double sqrtD = sqrt(d);
	double t1 = (-b - sqrtD)/(2*a);
	double t2 = (-b + sqrtD)/(2*a);
	
	// Check validity of t1 and t2
	double L = (B - A).length();
	Point atT1 = O + t1*D, atT2 = O + t2*D;
	bool t1Valid = (atT1.x >= 0 && atT1.x <= L);
	bool t2Valid = (atT2.x >= 0 && atT2.x <= L);
	double t;
	if(t1Valid) {
		if(t2Valid)
			// Use the lowest value
			t = (t1 <= t2 ? t1 : t2);
		else
			t = t1;
	} else {
		if(t2Valid)
			t = t2;
		else
			// Both are invalid, no intersection
			return Hit::NO_HIT();
	}
	
	// Compute the normal
	Point P = O + t*D;
	Vector N = P - Vector(P.x, 0, 0);
	
	// Transform the normal back to the world system
	N = (invRotToX*N + A).normalized();
	
	// If the intersection was on the inside of the cylinder,
	// flip the normal
	if (!t1Valid)
		N = -N;
	
	return Hit(t,N, this);
}

Point Cylinder::getRotationCenter()
{
	// The middle of the line from a to b
	return (A + B)/2;
}