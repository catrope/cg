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
	Image *texture, *specularTexture, *bumpmap, *photonmap, *photonblurmap;
	double bumpfactor;
	
	Object(const Vector &rotationVector, double rotationAngle) :
		r(Matrix::rotationDeg(rotationVector, rotationAngle)),
		rInv(Matrix::rotationDeg(rotationVector, -rotationAngle))
	{
		material = NULL;
		texture = NULL;
		specularTexture = NULL;
		bumpmap = NULL;
		photonmap = NULL;
		photonblurmap = NULL;
		bumpfactor = 1.0;
	}

	virtual ~Object()
	{
		if (texture)
			delete texture;
		if (specularTexture)
			delete specularTexture;
		if (bumpmap)
			delete bumpmap;
		if (material)
			delete material;
		if (photonmap)
			delete photonmap;
		if (photonblurmap)
			delete photonblurmap;
	}
	
	/**
	 * Intersect a ray with sub-objects.
	 * @param ray Ray to intersect with all objects
	 * @param closest If true, make sure to return the closest intersection. If false, stop at the first intersection
	 * @param maxT Ignore intersections with a t value greater than or equal to this
	 * @return Hit object
	 */
	virtual Hit intersect(const Ray &ray, bool closest, double maxT) = 0;
	
	
	virtual Point getRotationCenter() = 0;
	// TODO: Implement these three in Triangle and Quad and make them pure virtual
	virtual void getTexCoords(const Point &p, double &u, double &v) { u = 0; v = 0; }
	virtual Point getPointFromTexCoords(double u, double v) { return Point(0, 0, 0); }
	virtual double getRadius() { return 0.0; }
	
	Point rotate(const Point &p);
	Point unRotate(const Point &p);
	Color getColor(const Point &p);
	Color getPhotons(const Point &p);
	double getKs(const Point &p);
	Vector getBumpedNormal(const Vector &origNormal, const Point &p);
	void addPhoton(const Point &p, Color &color);
	void blurPhotonMap(int radius);

private:
	Matrix r, rInv; // rotation matrices
};

#endif /* end of include guard: OBJECT_H_AXKLE0OF */
