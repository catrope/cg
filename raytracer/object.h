//
//  Framework for a raytracer
//  File: object.h
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

#ifndef OBJECT_H_AXKLE0OF
#define OBJECT_H_AXKLE0OF

#include "triple.h"
#include "light.h"
#include "image.h"
#include "matrix.h"
#include "material.h"
#include "light.h"

class Object {
public:
	Material *material;
	Image *texture;
	
	Object(const Vector &rotationVector, double rotationAngle) :
		r(Matrix::rotationDeg(rotationVector, rotationAngle)),
		rInv(Matrix::rotationDeg(rotationVector, -rotationAngle))
	{
		material = NULL;
		texture = NULL;
	}

	virtual ~Object()
	{
		if (texture)
			delete texture;
		if (material)
			delete material;
	}

	virtual Hit intersect(const Ray &ray) = 0;
	virtual Point getRotationCenter() = 0;
	virtual void getTexCoords(const Point &p, double &u, double &v) { u = 0; v = 0; }
	
	Point rotate(const Point &p);
	Point unRotate(const Point &p);
	Color getColor(const Point &p);

private:
	Matrix r, rInv; // rotation matrices
};

#endif /* end of include guard: OBJECT_H_AXKLE0OF */
