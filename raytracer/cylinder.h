//
//  Framework for a raytracer
//  File: cylinder.h
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

#ifndef CYLINDER_H
#define CYLINDER_H

#include "object.h"

class Cylinder : public Object
{
public:
	Cylinder(Point axisStart, Point axisEnd, double radius, Vector &rot, double angle) :
			Object(rot, angle), A(axisStart), B(axisEnd), r(radius) {
		// FIXME: Doesn't take rotations into account
		// Precompute rotations needed for intersect()
		rotToX = Matrix::rotationOntoXAxis(B - A, &invRotToX);
	}

	virtual Hit intersect(const Ray &ray, bool closest, double maxT);
	virtual Point getRotationCenter();
	// TODO: Textures
	//virtual void getTexCoords(const Point &p, double &x, double &y);
	//virtual Point getPointFromTexCoords(double u, double v);

	const Point A, B;
	const double r;
	
	double getRadius() { return r; };

private:
	Matrix rotToX, invRotToX;
};

#endif /* end of include guard: CYLINDER_H */
