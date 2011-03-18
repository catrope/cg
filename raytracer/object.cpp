//
//  Framework for a raytracer
//  File: object.cpp
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Authors:
//    Roan Kattouw
//    Jan Paul Posma
//
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//

#include "object.h"
#include "material.h"

Point Object::rotate(const Point &p)
{
	/* We need to:
	 * 1) translate the rotation center to the origin
	 * 2) rotate rAngle degrees around rVec
	 * 3) translate back (undo #1)
	 */
	Point c = getRotationCenter();
	return r*(p - c) + c;
}

Point Object::unRotate(const Point &p)
{
	Point c = getRotationCenter();
	return rInv*(p - c) + c;
}

Color Object::getColor(const Point &p)
{
	if (!texture || texture->size() == 0)
		// No texture, use material
		return material->color;
	
	double u, v;
	getTexCoords(p, u, v);
	// u and v are in [0,1] so scale them to the texture dimensions
	return texture->colorAt(u, v);
}
