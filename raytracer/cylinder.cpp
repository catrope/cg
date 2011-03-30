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
	bool t1Valid = t1 >= 0 && (atT1.x >= 0 && atT1.x <= L);
	bool t2Valid = t2 >= 0 && (atT2.x >= 0 && atT2.x <= L);
	double t;
	
	/* Our cylinders are massive, not hollow.
	 * If the ray intersects the circle around A or B, we care
	 * about that too.
	 * These intersections are detected by checking if a ray enters
	 * the cylinder on one side of A and exits on the other side.
	 * If so, we adjust the relevant tN and atTN values to match the
	 * intersection point with the x=0 plane (for A) or x=L plane (for B),
	 * which will automatically be inside the circle because it's between
	 * the entry and exit points.
	 */
	if (atT1.x < 0 && atT2.x >= 0) {
		// Set t1 to where the ray intersects the x=0 plane
		// This will happen within the cylinder because atT2.x >= 0
		t1 += -atT1.x / D.x;
		atT1 = O + t1*D;
		t1Valid = t1 >= 0;
	}
	if (atT2.x < 0 && atT1.x >= 0) {
		// Same as above, with t1 and t2 reversed
		t2 += -atT2.x / D.x;
		atT2 = O + t2*D;
		t2Valid = t2 >= 0;
	}
	if (atT1.x <= L && atT2.x > L) {
		// Set t2 to where the ray intersects the x=L plane
		// This will happen within the cylinder because atT1.x <= L
		t2 -= (atT2.x - L) / D.x;
		atT2 = O + t2*D;
		t2Valid = t2 >= 0;
	}
	if (atT2.x <= L && atT1.x > L) {
		// Same as above, with t1 and t2 reversed
		t1 -= (atT1.x - L) / D.x;
		atT1 = O + t1*D;
		t1Valid = t1 >= 0;
	}
	
	
	// If both t1 and t2 are invalid, there is no intersection
	if (!t1Valid && !t2Valid)
		return Hit::NO_HIT();
	// Choose the lowest valid t
	t = t1Valid && t1 <= t2 ? t1 : t2;
	
	// Compute the normal
	Vector N;
	Point P = O + t*D;
	Point Pproj(P.x, 0, 0);
	if (fabs((P - Pproj).length_2() - r*r) < 0.01)
		// P is on the cylinder
		N = P - Pproj;
	// else P is on a bounding circle. Figure out which one
	else if (Pproj.x - L/2 < 0)
		// P is on the circle around A
		N = B - A;
	else
		// P is on the circle around B
		N = A - B;
	
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