//
//  Framework for a raytracer
//  File: instance.cpp
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
#include "instance.h"
#include "hit.h"
#include "ray.h"
#include <string>
#include <vector>

std::map<std::string, Instance*> Instance::map;

Instance::Instance(Point pos, const std::string &name) : Object(Vector(0, 0, 1), 0.0), position(pos)
{
	 if (Instance::map.count(name))
	 {
	 	objects = Instance::map[name]->objects;
	 	material = Instance::map[name]->material;
	 }
	 else
	 {
	 	objects = new std::vector<Object*>();
	 	Instance::map[name] = this;
	 }
}

Hit Instance::intersect(const Ray &ray, bool closest, double maxT)
{
	// Find hit object and distance
	Hit min_hit = Hit::NO_HIT();
	
	Ray r(ray.O - position, ray.D);
	
	for (unsigned int i = 0; i < objects->size(); ++i) {
		Hit hit = (*objects)[i]->intersect(r, closest, maxT);
		if (hit.hasHit() && (hit.t < min_hit.t || !min_hit.hasHit()) && hit.t < maxT) {
			min_hit = hit;
			if (!closest)
				break;
		}
	}
	
	if (material) min_hit.makeObj(this);
	
	return min_hit;
}