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

#include "sphere.h"
#include <iostream>
#include <math.h>

/************************** Sphere **********************************/

Hit Sphere::intersect(const Ray &ray)
{
	/****************************************************
	* RT1.1: INTERSECTION CALCULATION
	*
	* Given: ray, position, r
	* Sought: intersects? if true: *t
	* 
	* Insert calculation of ray/sphere intersection here. 
	*
	* You have the sphere's center (C) and radius (r) as well as
	* the ray's origin (ray.O) and direction (ray.D).
	*
	* If the ray does not intersect the sphere, return false.
	* Otherwise, return true and place the distance of the
	* intersection point from the ray origin in *t (see example).
	****************************************************/
	
	// t = (D . (C-O)) +/- sqrt( (D . (C-O))^2 - (C-O)^2 + r^2 )
	// where C=position
	Vector CO = position - ray.O;
	double DdotCO = ray.D.dot(CO);
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

	/****************************************************
	* RT1.2: NORMAL CALCULATION
	*
	* Given: t, C, r
	* Sought: N
	* 
	* Insert calculation of the sphere's normal at the intersection point.
	****************************************************/

	Vector N = (ray.at(t) - position).normalized();

	return Hit(t,N);
}
