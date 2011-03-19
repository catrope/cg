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
#include <cstdlib>

Color Scene::trace(const Ray &ray, unsigned int recursionDepth)
{
	Hit min_hit = intersectRay(ray, true, std::numeric_limits<double>::infinity());
	if (!min_hit.hasHit())
		// No hit? Return background color
		return Color(0.0, 0.0, 0.0);

	Point hit = ray.at(min_hit.t); //the hit point
	Object *obj = min_hit.obj;
	Vector N = obj->getBumpedNormal(min_hit.N, hit); //the normal at hit point
	Vector V = -ray.D; //the view vector
	
	switch (mode)
	{
		case zbuffer:
			return Color(min_hit.t/1000, min_hit.t/1000, min_hit.t/1000);
		case normal:
			return N/2+0.5;
		case texcoords:
			double u, v;
			obj->getTexCoords(hit, u, v);
			return Color(u, v, 0);
		case gooch:
			return calcGooch(obj, &hit, &N, &V, recursionDepth);
		case phong:
		default:
			return calcPhong(obj, &hit, &N, &V, recursionDepth);
	}
}

/**
 * Intersect a ray with all objects in the scene.
 * @param ray Ray to intersect with all objects
 * @param closest If true, make sure to return the closest intersection. If false, stop at the first intersection
 * @param maxT Ignore intersections with a t value greater than or equal to this
 * @return Hit object
 */
Hit Scene::intersectRay(const Ray &ray, bool closest, double maxT)
{
	// Find hit object and distance
	Hit min_hit = Hit::NO_HIT();
	
	for (unsigned int i = 0; i < objects.size(); ++i) {
		Hit hit = objects[i]->intersect(ray);
		if (hit.hasHit() && (hit.t < min_hit.t || !min_hit.hasHit()) && hit.t < maxT) {
			min_hit = hit;
			if (!closest)
				break;
		}
	}
	
	return min_hit;
}

inline Vector Scene::reflectVector(Vector *N, Vector *V)
{
	// Compute Vrefl, the reflected vector of V
	return -1*(*V) + 2*(*V).dot(*N)*(*N); // -V + 2(V.N)N
}

inline void Scene::reflect(Color *color, Object *obj, Point *hit, Vector *N, Vector *V, double ks, unsigned int recursionDepth)
{	
	// Trace a ray from this position along Vrefl, then treat
	// the result as the color of an incoming light ray
	// along Vrefl, and calculate its specular component.

	// To prevent roundoff errors causing the reflected
	// ray to start below the reflection surface and
	// intersect it immediately, move the starting point
	// away from the surface (i.e. along Vrefl) a tiny bit.
	if (ks > 0)
	{
		Vector Vrefl = reflectVector(N, V);
		Ray reflected(*hit + 0.01*Vrefl, Vrefl);
		Color reflection = trace(reflected, recursionDepth + 1);
		*color += ks * reflection;
	}
}

inline Vector Scene::refractVector(Object *obj, Point *hit, Vector *N, Vector *V, double nOut, double nIn)
{
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
	
	if (vcHit.hasHit()) {
		// Intersection found. We're entering
		n1 = nOut;
		n2 = nIn;
	} else {
		// No intersection. We're exiting
		n1 = nIn;
		n2 = nOut;
	}
	
	// cos phi_1 = N.V
	double cosphi1 = (*N).dot(*V);
	// cos phi_2 = sqrt( 1 - (n1/n2)^2 * (1 - cosphi1^2) )
	double inRoot = 1 - ((n1/n2)*(n1/n2) * (1 - cosphi1*cosphi1));
	if (inRoot < 0) {
		// Total reflection
		return reflectVector(N, V);
	} else if (cosphi1 >= 0) {
		// (n1/n2)(-V) + ( (n1/n2)cosphi1 - cosphi2 )N
		return (n1/n2)*(-*V) + ((n1/n2)*cosphi1 - sqrt(inRoot))*(*N);
	} else {
		// (n1/n2)(-V) + ( (n1/n2)cosphi1 + cosphi2 )N
		return (n1/n2)*(-*V) + ((n1/n2)*cosphi1 + sqrt(inRoot))*(*N);
	}
}

inline void Scene::refract(Color *color, Object *obj, Point *hit, Vector *N, Vector *V, unsigned int recursionDepth)
{
	if (obj->material->refract >= 0.01) {
		Vector T = refractVector(obj, hit, N, V, 1.0, obj->material->eta);
		
		// Like with reflection, trace a ray along T and guard
		// against roundoff errors
		Ray refracted(*hit + 0.01*T, T);
		Color refraction = trace(refracted, recursionDepth + 1);
		
		// Blend the refracted color in
		*color = (1 - obj->material->refract)*(*color) + obj->material->refract*refraction;
	}
}

inline bool Scene::shadowed(Object *obj, Light *light, Vector *L)
{
	// Construct an object for the incoming light ray and check
	// whether it intersects any other objects before this one
	Ray lightRay(light->position, -1*(*L));
	Hit ourHit = obj->intersect(lightRay);
	return intersectRay(lightRay, false, ourHit.t).hasHit();
}

inline void Scene::diffusePhong(Color *color, Object *obj, Point *hit, Light *light, Vector *L, Vector *N)
{
	// Diffuse lighting
	double NL = N->dot(*L);
	// If the dot product is negative, the light is not
	// visible to the viewer
	if (NL >= 0) {
		*color += obj->material->kd * obj->getColor(*hit) * light->color * NL;
	}
}

inline void Scene::diffuseGooch(Color *color, Object *obj, Point *hit, Light *light, Vector *L, Vector *N, Vector *V)
{
	/* Gooch lighting */
	double NL = N->dot(*L);
	
	Color diffuse = obj->material->kd * obj->getColor(*hit) * light->color;
	
	Color kCool = Color(0.0, 0.0, goochB) + goochAlpha*diffuse;
	Color kWarm = Color(goochY, goochY, 0.0) + goochBeta*diffuse;
	*color += kCool * (1 - NL)/2 + kWarm * (1 + NL)/2;
}

inline void Scene::specular(Color *color, Object *obj, Light *light, Vector *L, Vector *N, Vector *V, double ks)
{
	// Specular lighting
	Vector R = -1*(*L) + 2*L->dot(*N)*(*N); // R = -L + 2(L.N)N
	double VR = V->dot(R);
	// Skip negative dot products, see above.
	// We also don't want negative exponents
	if (VR >= 0 && obj->material->n > 0 && ks > 0) {
		*color += ks * light->color * pow(VR, obj->material->n);
	}
}

inline void Scene::ambient(Color *color, Object *obj, Point *hit)
{
	*color += obj->material->ka * obj->getColor(*hit) * globalAmbient;
}

inline Vector Scene::lightVector(Point *hit, Light *light)
{
	return (light->position - *hit).normalized();
}

Color Scene::calcPhong(Object *obj, Point *hit, Vector *N, Vector *V, unsigned int recursionDepth)
{
	Color color(0.0, 0.0, 0.0);
	double ks = obj->getKs(*hit);
	
	ambient(&color, obj, hit);
	
	for (unsigned int i = 0; i < lights.size(); i++) {
		// Normalized vector from the surface to the light source,
		// i.e. the reversed direction of the incoming light ray
		Vector L = lightVector(hit, lights[i]);
		
		// If this light ray is shadowed from this object by some other
		// object, ignore it.
		if (shadowed(obj, lights[i], &L) && shadows) {
			continue;
		}
		
		diffusePhong(&color, obj, hit, lights[i], &L, N);
		specular(&color, obj, lights[i], &L, N, V, ks);
	}
	
	// Reflection and refraction
	if (recursionDepth < maxRecursionDepth) {
		reflect(&color, obj, hit, N, V, ks, recursionDepth);
		refract(&color, obj, hit, N, V, recursionDepth);
	}
	
	return color;
}

Color Scene::calcGooch(Object *obj, Point *hit, Vector *N, Vector *V, unsigned int recursionDepth)
{
	Color color(0.0, 0.0, 0.0);
	double ks = obj->getKs(*hit);
	
	for (unsigned int i = 0; i < lights.size(); i++) {
		// Normalized vector from the surface to the light source,
		// i.e. the reversed direction of the incoming light ray
		Vector L = lightVector(hit, lights[i]);
		
		// If this light ray is shadowed from this object by some other
		// object, ignore it.
		if (shadowed(obj, lights[i], &L) && shadows) {
			continue;
		}
		
		diffuseGooch(&color, obj, hit, lights[i], &L, N, V);
		specular(&color, obj, lights[i], &L, N, V, ks);
	}
	
	// Reflection and refraction
	if (recursionDepth < maxRecursionDepth) {
		reflect(&color, obj, hit, N, V, ks, recursionDepth);
		refract(&color, obj, hit, N, V, recursionDepth);
	}
	
	return color;
}

inline Color Scene::anaglyphRay(Point pixel, Point eye)
{
	if (camera.anaglyph)
	{
		Point leftEye = eye - camera.eyesOffset;
		Point rightEye = eye + camera.eyesOffset;
		Ray leftRay(leftEye, (pixel-leftEye).normalized());
		Ray rightRay(rightEye, (pixel-rightEye).normalized());
		Color leftCol = trace(leftRay, 0);
		Color rightCol = trace(rightRay, 0);
		if (camera.grey)
		{
			leftCol.set(leftCol.r + leftCol.g + leftCol.b, 3);
			rightCol.set(rightCol.r + rightCol.g + rightCol.b, 3);
		}
		
		Color finalCol(leftCol.r, rightCol.g, rightCol.b);
		return finalCol;
	}
	else
	{
		Ray ray(eye, (pixel-eye).normalized());
		Color finalCol = trace(ray, 0);
		return finalCol;
	}
}

inline Color Scene::exposureRay(Point pixel, Point eye)
{
	Color col(0,0,0);
	
	for (unsigned int i = 0; i < camera.exposureSamples; i++)
	{
		double time = (double)i * camera.exposureTime / (double)camera.exposureSamples;
		Point motionEye = eye + camera.velocity*time + camera.acceleration*time*time/2.0;
		col += anaglyphRay(pixel, motionEye);
	}
	col /= camera.exposureSamples;
	return col;
}

inline Color Scene::apertureRay(Point pixel, unsigned int subpixel)
{
	Color col(0,0,0);
	
	// calculate the vectors for moving one unit right or down
	Vector xvec = (camera.center-camera.eye).normalized().cross(camera.up).normalized();
	Vector yvec = -camera.up.normalized();
	
	double roffset = fmod((double)subpixel*1.61803399, 1.0);
	
	for (unsigned int i = 0; i < camera.apertureSamples; i++)
	{
		double r = sqrt((double)i + roffset)*camera.apertureRadius/(camera.up.length()*sqrt(camera.apertureSamples));
		double theta = (double)(i + subpixel) * 2.399963;
		
		Point eye = camera.eye + xvec*r*cos(theta) + yvec*r*sin(theta);
		col += exposureRay(pixel, eye);
	}
	col /= camera.apertureSamples;
	return col;
}

inline Color Scene::superSampleRay(Point origPixel, Vector xvec, Vector yvec, unsigned int factor)
{
	Color col(0,0,0);
	
	// special case to prevent division by zero
	if (factor == 1)
	{
		col = apertureRay(origPixel, 0);
	}
	else
	{
		xvec /= ((double)factor-1.0);
		yvec /= ((double)factor-1.0);
		unsigned int subpixel = 0;
		
		#pragma omp parallel for
		for (int y = 0; y < (int)factor; y++)
		{
			#pragma omp parallel for
			for (int x = 0; x < (int)factor; x++)
			{
				Vector xoffset = xvec*(double)x - xvec*(double)factor/2.0;
				Vector yoffset = yvec*(double)y - yvec*(double)factor/2.0;
					
				if (superSamplingJitter)
				{
					xoffset += xvec*((double)rand()/(double)RAND_MAX - 0.5);
					yoffset += yvec*((double)rand()/(double)RAND_MAX - 0.5);
				}
				
				Point pixel = origPixel + xoffset + yoffset;
				col += apertureRay(pixel, subpixel++);
			}
		}
		col /= factor*factor;
	}
	
	col.clamp();
	return col;
}

void Scene::computeGlobalAmbient()
{
	globalAmbient = Color(0.0, 0.0, 0.0);
	
	for (unsigned int i = 0; i < lights.size(); i++) {
		// This light's contribution to ambient lighting
		globalAmbient += lights[i]->color;
	}
	
	globalAmbient.clamp();
}

void Scene::render(Image &img)
{
	int w = camera.viewWidth;
	int h = camera.viewHeight;
	
	// calculate the vectors for moving one pixel right or down
	Vector xvec = (camera.center-camera.eye).normalized().cross(camera.up);
	Vector yvec = -camera.up;
	
	// init random generator
	srand(time(NULL));
	
	computeGlobalAmbient();
	
	#pragma omp parallel for
	for (int y = 0; y < h; y++)
	{
		#pragma omp parallel for
		for (int x = 0; x < w; x++)
		{
			Point pixel = camera.center
				+ yvec*(double)y - yvec*(double)h/2.0
				+ xvec*(double)x - xvec*(double)w/2.0;
			img(x,y) = superSampleRay(pixel, xvec, yvec, superSamplingFactor);
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
	camera.center = Point(e.x, e.y, 0);
	camera.up = Vector(0,1,0);
	camera.viewWidth = 400;
	camera.viewHeight = 400;
}
