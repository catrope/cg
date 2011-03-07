//
//  Framework for a raytracer
//  File: scene.cpp
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

#include "scene.h"
#include "material.h"
#include <cstdio>

Color Scene::trace(const Ray &ray, unsigned int recursionDepth)
{
	// Find hit object and distance
	Hit min_hit(std::numeric_limits<double>::infinity(),Vector());
	
	Object *obj = NULL;
	for (unsigned int i = 0; i < objects.size(); ++i) {
		Hit hit(objects[i]->intersect(ray));
		if (hit.t<min_hit.t) {
			min_hit = hit;
			obj = objects[i];
		}
	}

	// No hit? Return background color.
	if (!obj) return Color(0.0, 0.0, 0.0);

	Point hit = ray.at(min_hit.t); //the hit point
	Vector N = min_hit.N; //the normal at hit point
	Vector V = -ray.D; //the view vector
	
	switch (mode)
	{
		case zbuffer:
			return Color(min_hit.t/1000, min_hit.t/1000, min_hit.t/1000);
		case normal:
			return N/2+0.5;
		case phong:
		default:
			return calcPhong(obj, &hit, &N, &V, recursionDepth);
	}
}

Color Scene::calcPhong(Object *obj, Point *hit, Vector *N, Vector *V, unsigned int recursionDepth)
{
	// Apply Phong lighting model
	// Formulas are described in section 10.2.1 of "Fundamentals of CG", 3rd Ed.
	Color color(0.0, 0.0, 0.0);
	Color ambient(0.0, 0.0, 0.0); // accumulator for ambient light intensity
	
	for (unsigned int i = 0; i < lights.size(); i++) {
		// Normalized vector from the surface to the light source,
		// i.e. the reversed direction of the incoming light ray
		Vector L = (lights[i]->position - *hit).normalized();
		
		// Construct an object for the incoming light ray and check
		// whether it intersects any other objects before this one
		Ray lightRay(lights[i]->position, -1*L);
		Hit ourHit = obj->intersect(lightRay);
		bool shadowed = false;
		for (unsigned int j = 0; j < objects.size(); j++) {
			if (objects[j] != obj) {
				Hit hit = objects[j]->intersect(lightRay);
				if (hit.t < ourHit.t) {
					shadowed = true;
					break;
				}
			}
		}
		
		// This light's contribution to ambient lighting
		// Don't calculate this for reflections
		if (recursionDepth == 0 || 1==1) {
			ambient += lights[i]->color;
		}
		
		// If this light ray is shadowed from this object by some other
		// object, ignore it. We still need this light's contribution
		// to ambient lighting, though, which is why this check has to
		// be in this exact place.
		if (shadowed) {
			continue;
		}
		
		// Diffuse lighting
		// Don't compute diffuse lighting for reflections
		if (recursionDepth == 0 || 1==1) {
			double NL = N->dot(L);
			// If the dot product is negative, the light is not
			// visible to the viewer
			if (NL >= 0) {
				color += obj->material->kd * obj->material->color * lights[i]->color * NL;
			}
		}
		
		// Specular lighting
		Vector R = -1*L + 2*L.dot(*N)*(*N); // R = -L + 2(L.N)N
		double VR = V->dot(R);
		// Skip negative dot products, see above.
		// We also don't want negative exponents
		if (VR >= 0 && obj->material->n > 0) {
			color += obj->material->ks * lights[i]->color * pow(VR, obj->material->n);
		}
		
		// Reflections
		if (recursionDepth < maxRecursionDepth) {
			// Trace a ray from this position along R, then treat
			// the result as the color of an incoming light ray
			// along R, and calculate its specular component.
			
			// We need VL instead of VR, because L is the reflected
			// vector of R.
			double VL = V->dot(L);
			if (VL >= 0 && obj->material->n > 0) {
				// To prevent roundoff errors causing the reflected
				// ray to start below the reflection surface and
				// intersect it immediately, move the starting point
				// away from the surface (i.e. along R) a tiny bit.
				Ray reflected(*hit + 0.01*R, R);
				Color reflection = trace(reflected, recursionDepth + 1);
				color += obj->material->ks * reflection * pow(VL, obj->material->n);
			}
		}
	}
	
	// Ambient lighting
	ambient.clamp();
	color += obj->material->ka * obj->material->color * ambient;
	
	return color;
}

void Scene::render(Image &img)
{
	int w = img.width();
	int h = img.height();
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			Point pixel(x+0.5, h-1-y+0.5, 0);
			Ray ray(eye, (pixel-eye).normalized());
			Color col = trace(ray, 0);
			col.clamp();
			img(x,y) = col;
		}
	}
}

void Scene::addObject(Object *o)
{
	objects.push_back(o);
}

void Scene::addLight(Light *l)
{
	lights.push_back(l);
}

void Scene::setEye(Triple e)
{
	eye = e;
}
