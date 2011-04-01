//
//  Framework for a raytracer
//  File: instance.h
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

#ifndef INSTANCE_H
#define INSTANCE_H

#include "object.h"
#include "triple.h"
#include <vector>
#include <string>
#include <map>

class Instance : public Object
{
public:
	Instance(Point pos, const std::string &name);
	~Instance() { delete objects; }
	
	virtual Hit intersect(const Ray &ray, bool closest, double maxT);
	virtual Point getRotationCenter() { return position; }
	void addObject(Object *o) { objects->push_back(o); }
	
	Point position;
	std::vector<Object*> *objects;
	static std::map<std::string, Instance*> map;
};

#endif /* end of include guard: INSTANCE_H */
