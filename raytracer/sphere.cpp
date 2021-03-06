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

#include "sphere.h"
#include <iostream>
#include <math.h>
#include <vector>

/************************** Sphere **********************************/

Hit Sphere::intersect(const Ray &ray, bool closest, double maxT)
{
	/* t = (l . c) +/- sqrt( (l . c)^2 - c^2 + r^2 )
	 * Formula and derivation found at https://secure.wikimedia.org/wikipedia/en/wiki/Line%E2%80%93sphere_intersection
	 * In this formula, l is the direction vector of the ray,
	 * c is the center of the circle, and r is its radius. The origin of
	 * the ray is assumed to be at (0,0,0). Removing the latter assumption
	 * and substituting the right letters, we get:
	 * t = (D . (C-O)) +/- sqrt( (D . (C-O))^2 - (C-O)^2 + r^2 )
	 * where C=position, O=ray.O and D=ray.D
	 */
	Vector CO = position - ray.O;
	double DdotCO = ray.D.dot(CO); // D.(C-O)
	double inRoot = DdotCO * DdotCO - CO.length_2() + r*r;
	if (inRoot < 0) {
		// No solutions
		return Hit::NO_HIT();
	}
	double t1 = DdotCO - sqrt(inRoot);
	double t2 = DdotCO + sqrt(inRoot);
	
	// Negative values for t are invalid. If both solutions are negative,
	// there is no "real" intersection
	if (t1 < 0 && t2 < 0) {
		return Hit::NO_HIT();
	}
	// Return the lowest value in {t1, t2} that is non-negative
	double t = (t1 <= t2 && t1 >= 0) ? t1 : t2;

	// Calculate surface normal vector. This is simply the vector from
	// the center of the sphere to the intersection point, normalized
	// to length 1.
	Vector N = (ray.at(t) - position).normalized();
	
	// ...except if the intersection is on the inside of the sphere, in
	// which case the normal needs to be flipped
	if (t1 < 0) {
		N = -N;
	}

	return Hit(t,N, this);
}

void Sphere::getTexCoords(const Point &p, double &u, double &v)
{
	// Texture mapping for spheres. Formulas from Fundamentals of CG p. 251
	Point pr = unRotate(p);
	double theta = acos((pr.z - position.z)/r);
	double phi = atan2(pr.y - position.y, pr.x - position.x)/* - (angle*M_PI/180.0)*/;
	while (phi < 0.0)
		phi += 2*M_PI;
	u = phi/(2*M_PI);
	// The book says v = (M_PI - theta)/M_PI but that results in mirrored v coords
	v = theta/M_PI;
}

Point Sphere::getPointFromTexCoords(double u, double v)
{
	double phi = u*2*M_PI;
	double theta = v*M_PI;
	if (phi > M_PI)
		phi -= 2*M_PI;
	return rotate(Point(
		position.x + r*cos(phi)*sin(theta),
		position.y + r*sin(phi)*sin(theta),
		position.z + r*cos(theta)
	));
}
