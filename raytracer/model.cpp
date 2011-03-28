//
//  Framework for a raytracer
//  File: model.cpp
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

#include "model.h"
#include "sphere.h"
#include "triangle.h"
#include "glm.h"

void Model::init(const std::string& filename, const Vector &rot, double angle)
{
	GLMmodel *model = glmReadOBJ((char *)filename.c_str());
	glmUnitize(model);
	
	// We want to use a bounding sphere instead of a bounding cube,
	// which is what glmUnitize does; therefore, scale down with sqrt(2)
	glmScale(model, (GLfloat)size/1.5);
	
	unsigned int cnt = 0;
	double *arr = glmModelDoubleArray(model, &cnt);
	
	triangles.reserve(cnt);
	for (unsigned int i=0; i<cnt; i++)
	{
		Triangle *tri = new Triangle(
			Point(arr[i*9+0], arr[i*9+1], arr[i*9+2]) + position,
			Point(arr[i*9+3], arr[i*9+4], arr[i*9+5]) + position,
			Point(arr[i*9+6], arr[i*9+7], arr[i*9+8]) + position
		);
		triangles.push_back(tri);
	}
	
	free(arr);
	
	boundingSphere = new Sphere(position, size, rot, angle);
}

Hit Model::intersect(const Ray &ray, bool closest, double maxT)
{	
	// First check the bounding sphere
	Hit bounding_hit = boundingSphere->intersect(ray, closest, maxT);
	if (!bounding_hit.hasHit()) return Hit::NO_HIT();
	
	// Find hit object and distance
	Hit min_hit = Hit::NO_HIT();
	
	for (unsigned int i = 0; i < triangles.size(); ++i) {
		Hit hit = triangles[i]->intersect(ray, closest, maxT);
		if (hit.hasHit() && (hit.t < min_hit.t || !min_hit.hasHit()) && hit.t < maxT) {
			min_hit = hit;
			if (!closest)
				break;
		}
	}
	
	min_hit.makeObj(this);
	return min_hit;
}

void Model::getTexCoords(const Point &p, double &u, double &v)
{
	boundingSphere->getTexCoords(p, u, v);
}
