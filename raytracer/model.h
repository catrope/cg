//
//  Framework for a raytracer
//  File: model.h
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

#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>
#include "object.h"
#include "sphere.h"
#include "triangle.h"

class Model : public Object
{
public:
	Model(Point pos, const std::string& filename, double size, const Vector &rot, double angle)
		: Object(rot, angle), position(pos), size(size) { init(filename, rot, angle); }
	
	Model(Point pos, const std::string& filename, double size)
		: Object(Vector(0, 0, 1), 0.0), position(pos), size(size) { init(filename, Vector(0, 0, 1), 0.0); }
		
	std::vector<Triangle*> triangles;
	Sphere * boundingSphere;

	virtual Hit intersect(const Ray &ray, bool closest, double maxT);
	virtual Point getRotationCenter() { return position; }
	virtual void getTexCoords(const Point &p, double &x, double &y);

	const Point position;
	const double size;
	
	double getRadius() { return boundingSphere->getRadius(); };
	
private:
	void init(const std::string& filename, const Vector &rot, double angle);
};

#endif /* end of include guard: MODEL_H */
