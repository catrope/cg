//
//  Framework for a raytracer
//  File: hit.h
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

#ifndef HIT_H
#define HIT_H

#include <iostream>
#include <limits>
#include "triple.h"

class Object;

class Hit
{
public:
	double t;
	Vector N;
	Object *obj;

	Hit(const double t, const Vector &normal, Object *object)
		: t(t), N(normal), obj(object)
	{ }
	
	Hit() { Hit(std::numeric_limits<double>::infinity(),Vector(), NULL); }
	
	bool hasHit() { return obj != NULL; }
	void makeObj(Object *object) { if (hasHit()) obj = object; }

	static const Hit NO_HIT()
	{
		static Hit no_hit(std::numeric_limits<double>::infinity(), Vector(0, 0, 0), NULL);
		return no_hit;
	}
};

#endif /* end of include guard: HIT_H */
