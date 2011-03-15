//
//  Framework for a raytracer
//  File: scene.h
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

#ifndef SCENE_H_KNBLQLP6
#define SCENE_H_KNBLQLP6

#include <vector>
#include "triple.h"
#include "light.h"
#include "object.h"
#include "image.h"
#include "camera.h"

class Scene
{
private:
	std::vector<Object*> objects;
	std::vector<Light*> lights;
	Camera camera;
	bool shadows;
	unsigned int maxRecursionDepth;
	unsigned int superSamplingFactor;
	bool superSamplingJitter;
	Color calcPhong(Object *obj, Point *hit, Vector *N, Vector *V, unsigned int recursionDepth);
	Color anaglyphRay(Point pixel, Point eye);
	Color exposureRay(Point pixel, Point eye);
	Color superSampleRay(Vector origPixel, Vector xvec, Vector yvec, unsigned int factor);
	Color apertureRay(Vector pixel, unsigned int subpixel);
	bool intersectRay(const Ray &ray, Hit *h, Object **o, bool closest, double maxT);
public:	
	enum RenderMode {
		phong, zbuffer, normal
	} mode;
	
	Color trace(const Ray &ray, unsigned int recursionDepth);
	void render(Image &img);
	void addObject(Object *o);
	void addLight(Light *l);
	void setEye(Triple e);
	void setCamera(Camera c) { camera = c; }
	Camera getCamera() { return camera; }
	void setSuperSamplingFactor(unsigned int f) { superSamplingFactor = f; }
	void setSuperSamplingJitter(bool j) { superSamplingJitter = j; }
	void setRenderMode(Scene::RenderMode m) { mode = m; }
	void setShadows(bool b) { shadows = b; }
	void setMaxRecursionDepth(unsigned int d) { maxRecursionDepth = d; }
	unsigned int getNumObjects() { return objects.size(); }
	unsigned int getNumLights() { return lights.size(); }
};

#endif /* end of include guard: SCENE_H_KNBLQLP6 */
