//
//  Framework for a raytracer
//  File: object.h
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

#ifndef OBJECT_H_AXKLE0OF
#define OBJECT_H_AXKLE0OF

#include "triple.h"
#include "light.h"

class Material;

class Object {
public:
	Material *material;

	virtual ~Object() { }

	virtual Hit intersect(const Ray &ray) = 0;
	virtual void getTexCoords(const Point &p, double &u, double &v) { u = 0; v = 0; }
	
	void setTextureFile(const unsigned char *file);
	void setTextureFile(std::string file) { setTextureFile(file.c_str()); }

protected:
	Color *textureData;
};

#endif /* end of include guard: OBJECT_H_AXKLE0OF */
