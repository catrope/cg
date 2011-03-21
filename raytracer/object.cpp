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
	Color color(0,0,0);
	double u, v;
	getTexCoords(p, u, v);
	
	//if (!texture || texture->size() == 0)
		// No texture, use material
	//	color = material->color;
	//else
		// u and v are in [0,1] so scale them to the texture dimensions
	//	color = texture->colorAt(u, v);
		
	if (photonmap)
		color += photonmap->colorAt(u, v);
	
	return color;
}

double Object::getKs(const Point &p)
{
	if (!specularTexture || specularTexture->size() == 0)
		// No texture, use material
		return material->ks;
	
	double u, v;
	getTexCoords(p, u, v);
	// Use the red channel of the texture image, ignore the green and blue channels
	return material->ks*specularTexture->colorAt(u, v).r;
}

Vector Object::getBumpedNormal(const Vector &origNormal, const Point &p)
{
	if (!bumpmap || bumpmap->size() == 0)
		// No bump map, don't mess with normal
		return origNormal;
	
	double u, v;
	float dx, dy;
	getTexCoords(p, u, v);
	bumpmap->derivativeAt(u, v, &dx, &dy);
	
	// Get the vectors to (u,v) from the points corresponding to the texture pixels
	// immediately right and down from it
	Vector dxVec = p - getPointFromTexCoords(min(u+1.0/(bumpmap->width()-1), 1.0), v);
	Vector dyVec = p - getPointFromTexCoords(u, min(v+1.0/(bumpmap->height()-1), 1.0));

	return (origNormal + bumpfactor*(dx*dxVec + dy*dyVec)).normalized();
}

void Object::addPhoton(const Point &p, Color &color)
{
	if (photonmap)
	{
		double u, v;
		getTexCoords(p, u, v);
		photonmap->setColorAt(u, v, color + photonmap->colorAt(u, v));
	}
}