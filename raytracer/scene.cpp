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
		ambient += lights[i]->color;
		
		// If this light ray is shadowed from this object by some other
		// object, ignore it. We still need this light's contribution
		// to ambient lighting, though, which is why this check has to
		// be in this exact place.
		if (shadowed && shadows) {
			continue;
		}
		
		// Diffuse lighting
		double NL = N->dot(L);
		// If the dot product is negative, the light is not
		// visible to the viewer
		if (NL >= 0) {
			color += obj->material->kd * obj->material->color * lights[i]->color * NL;
		}
		
		// Specular lighting
		Vector R = -1*L + 2*L.dot(*N)*(*N); // R = -L + 2(L.N)N
		double VR = V->dot(R);
		// Skip negative dot products, see above.
		// We also don't want negative exponents
		if (VR >= 0 && obj->material->n > 0) {
			color += obj->material->ks * lights[i]->color * pow(VR, obj->material->n);
		}
	}
	
	// Ambient lighting
	ambient.clamp();
	color += obj->material->ka * obj->material->color * ambient;
	
	// Reflection and refraction
	if (recursionDepth < maxRecursionDepth) {
		// Reflection		
		// Compute Vrefl, the reflected vector of V
		Vector Vrefl = -1*(*V) + 2*(*V).dot(*N)*(*N); // -V + 2(V.N)N
		
		// Trace a ray from this position along Vrefl, then treat
		// the result as the color of an incoming light ray
		// along Vrefl, and calculate its specular component.

		// To prevent roundoff errors causing the reflected
		// ray to start below the reflection surface and
		// intersect it immediately, move the starting point
		// away from the surface (i.e. along Vrefl) a tiny bit.
		if (obj->material->ks > 0) {
			Ray reflected(*hit + 0.01*Vrefl, Vrefl);
			Color reflection = trace(reflected, recursionDepth + 1);
			color += obj->material->ks * reflection;
		}
		
		// Refraction
		if (obj->material->refract >= 0.01) {
			// Compute the transmission vector using Snell's law
			// Formulas from https://secure.wikimedia.org/wikipedia/en/wiki/Snell%27s_law#Vector_form
			// with L replaced with -V
			
			// If the ray is leaving the object rather than entering it,
			// we need to flip n1 and n2. We can detect this by constructing
			// a ray that runs along V and starts just past the hit point,
			// and intersecting that with the object again. If an
			// intersection is found, that means we're entering the object.
			Ray Vcont(*hit - 0.01*(*V), -*V);
			Hit vcHit = obj->intersect(Vcont);
			double n1, n2;
			Vector T;
			// Check for no hit with !(lcHit < infinity), which is what's
			// used above for finding the closest intersecting object
			if (vcHit.t < std::numeric_limits<double>::infinity()) {
				// Intersection found. We're entering
				n1 = 1;
				n2 = obj->material->eta;
			} else {
				// No intersection. We're exiting
				n1 = obj->material->eta;
				n2 = 1;
			}
			
			// cos phi_1 = N.V
			double cosphi1 = (*N).dot(*V);
			// cos phi_2 = sqrt( 1 - (n1/n2)^2 * (1 - cosphi1^2) )
			double inRoot = 1 - ((n1/n2)*(n1/n2) * (1 - cosphi1*cosphi1));
			if (inRoot < 0) {
				// Total reflection
				T = Vrefl;
			} else if (cosphi1 >= 0) {
				// T = (n1/n2)(-V) + ( (n1/n2)cosphi1 - cosphi2 )N
				T = (n1/n2)*(-*V) + ((n1/n2)*cosphi1 - sqrt(inRoot))*(*N);
			} else {
				// T = (n1/n2)(-V) + ( (n1/n2)cosphi1 + cosphi2 )N
				T = (n1/n2)*(-*V) + ((n1/n2)*cosphi1 + sqrt(inRoot))*(*N);
			}
			
			// Like with reflection, trace a ray along T and guard
			// against roundoff errors
			Ray refracted(*hit + 0.01*T, T);
			Color refraction = trace(refracted, recursionDepth + 1);
			
			// Blend the refracted color in
			color = (1 - obj->material->refract)*color + obj->material->refract*refraction;
		}
	}
	
	return color;
}

void Scene::render(Image &img)
{
	int w = camera.viewWidth;
	int h = camera.viewHeight;
	Vector xvec = (camera.center-camera.eye).normalized().cross(camera.up);
	Vector yvec = -camera.up;
	
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			Point pixel = camera.center
				+ yvec*(double)y - yvec*(double)h/2.0
				+ xvec*(double)x - xvec*(double)w/2.0;
			Ray ray(camera.eye, (pixel-camera.eye).normalized());
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
	camera.eye = e;
	camera.center = Vector(e.x, e.y, 0);
	camera.up = Vector(0,1,0);
	camera.viewWidth = 400;
	camera.viewHeight = 400;
}

void Scene::setCamera(Camera c)
{
	camera = c;
}

Camera Scene::getCamera()
{
	return camera;
}