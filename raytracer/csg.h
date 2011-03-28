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

#ifndef CSG_H
#define CSG_H

#include "object.h"

class Csg : public Object
{
public:
	enum Operation {
		opUnion, opIntersect, opDifference
	};
	
	Csg(Object *o1, Object *o2, Point pos, Csg::Operation op) : Object(Vector(0, 0, 1), 0.0), 
		o1(o1), o2(o2), position(pos), op(op)  { }

	virtual Hit intersect(const Ray &ray, bool closest, double maxT);
	virtual Point getRotationCenter() { return position; }
	
	Object *o1, *o2;
	Point position;
	Operation op;
	
private:
	Hit closestHit(const Ray &ray);
	Hit isect(Ray ray);
	Hit diff(Ray ray);
};

#endif /* end of include guard: CSG_H */
