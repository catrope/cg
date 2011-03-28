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
	double minRecursionWeight;
	unsigned int superSamplingFactor, superSamplingTotal, superSamplingMinFactor;
	double superSamplingThreshold;
	bool superSamplingJitter;
	Color globalAmbient;
	double goochB, goochY, goochAlpha, goochBeta;
	double edges;
	int photonFactor, photonBlur;
	double photonIntensity;
	unsigned int ambientFactor;
	double ambientRandom;
	
	Color calcPhong(Object *obj, Point *hit, Vector *N, Vector *V, unsigned int recursionDepth, double recursionWeight);
	Color calcGooch(Object *obj, Point *hit, Vector *N, Vector *V, unsigned int recursionDepth, double recursionWeight);
	
	inline Vector reflectVector(Vector *N, Vector *V);
	inline void reflect(Color *color, Object *obj, Point *hit, Vector *N, Vector *V, double ks, unsigned int recursionDepth, double recursionWeight);
	inline Vector refractVector(Object *obj, Point *hit, Vector *N, Vector *V, double nOut, double nIn);
	inline void refract(Color *color, Object *obj, Point *hit, Vector *N, Vector *V, unsigned int recursionDepth, double recursionWeight);
	inline bool shadowed(Object *obj, Light *light, Vector *L);
	inline void diffusePhong(Color *color, Object *obj, Point *hit, Light *light, Vector *L, Vector *N);
	inline void diffuseGooch(Color *color, Object *obj, Point *hit, Light *light, Vector *L, Vector *N, Vector *V);
	inline void specular(Color *color, Object *obj, Light *light, Vector *L, Vector *N, Vector *V, double ks);
	inline void ambient(Color *color, Object *obj, Point *hit, Vector *N);
	inline bool edgeDetection(Color *color, Vector *N, Vector *V);
	inline Vector lightVector(Point *hit, Light *light);
	inline void photons(Color *color, Object *obj, Point *hit);
	
	inline Color anaglyphRay(Point pixel, Point eye);
	inline Color exposureRay(Point pixel, Point eye);
	void superSampleRayRecursion(Color * totalCol, unsigned int * nPoints, unsigned int * subpixel, Point origPixel, Vector xvec, Vector yvec, unsigned int factor);
	inline Color superSampleRay(Point origPixel, Vector xvec, Vector yvec);
	inline Color apertureRay(Vector pixel, unsigned int subpixel);
	Hit intersectRay(const Ray &ray, bool closest, double maxT, bool traceLights);
	void computeGlobalAmbient();
	
	void tracePhoton(Color color, const Ray &ray, unsigned int recursionDepth, double recursionWeight, Object *onlyObject);
	void renderPhotonsForLightAndObject(Light *light, Object *obj);
	void renderPhotonsForLight(Light *light);
	void renderPhotons();
	void blurPhotonMaps();
	
public:	
	enum RenderMode {
		phong, zbuffer, normal, texcoords, gooch, ssdepth
	} mode;
	
	Color trace(const Ray &ray, unsigned int recursionDepth, double recursionWeight, bool traceLights);
	void render(Image &img);
	void addObject(Object *o);
	void addLight(Light *l);
	void setEye(Triple e);
	void setCamera(Camera c) { camera = c; }
	Camera getCamera() { return camera; }	
	void setSuperSampling(unsigned int f, unsigned int fmin, double threshold, bool jitter)
	{ superSamplingFactor = f; superSamplingMinFactor = fmin; superSamplingThreshold = threshold; 
	superSamplingJitter = jitter; superSamplingTotal = f*f; }
	void setRenderMode(Scene::RenderMode m) { mode = m; }
	void setShadows(bool b) { shadows = b; }
	void setEdges(double e) { edges = e; }
	void setMaxRecursionDepth(unsigned int d) { maxRecursionDepth = d; }
	void setMinRecursionWeight(double w) { minRecursionWeight = w; }
	void setPhotonFactor(unsigned int f) { photonFactor = (int)f; }
	void setPhotonBlur(unsigned int b) { photonBlur = (int)b; }
	void setPhotonIntensity(double i) { photonIntensity = i; }
	void setAmbient(unsigned int f, double r) { ambientFactor = f; ambientRandom = r; }
	unsigned int getNumObjects() { return objects.size(); }
	unsigned int getNumLights() { return lights.size(); }
	void setGoochParameters(double b, double y, double alpha, double beta)
	{ goochB = b; goochY = y; goochAlpha = alpha; goochBeta = beta; }
};

#endif /* end of include guard: SCENE_H_KNBLQLP6 */
