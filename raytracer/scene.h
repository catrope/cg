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

class Scene
{
private:
	std::vector<Object*> objects;
	std::vector<Light*> lights;
	Triple eye;
	bool shadows;
	unsigned int maxRecursionDepth;
	Color calcPhong(Object *obj, Point *hit, Vector *N, Vector *V);
public:
	enum RenderMode {
		phong, zbuffer, normal
	} mode;
	
	Color trace(const Ray &ray);
	void render(Image &img);
	void addObject(Object *o);
	void addLight(Light *l);
	void setEye(Triple e);
	void setRenderMode(Scene::RenderMode m) { mode = m; }
	void setShadows(bool b) { shadows = b; }
	void setMaxRecursionDepth(unsigned int d) { maxRecursionDepth = d; }
	unsigned int getNumObjects() { return objects.size(); }
	unsigned int getNumLights() { return lights.size(); }
};

#endif /* end of include guard: SCENE_H_KNBLQLP6 */
