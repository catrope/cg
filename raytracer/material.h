//
//  Framework for a raytracer
//  File: material.h
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Authors:
//    Maarten Everts
//    Roan Kattouw
//    Jan Paul Posma
//
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//

#ifndef MATERIAL_H_TWMNT2EJ
#define MATERIAL_H_TWMNT2EJ

#include <iostream>
#include "triple.h"

class Material
{
public:
	Color color;		// base color
	double ka;		// ambient intensity
	double kd;		// diffuse intensity
	double ks;		// specular intensity 
	double n;		// exponent for specular highlight size
	double refract;		// weighting factor for refraction (0..1)
	double eta;		// refraction index
	bool light; // is this a light

	Material() { ka=0.0; kd=0.0; ks=0.0; n=0.0; refract=0.0; eta=0.0; light=false; }
};

#endif /* end of include guard: MATERIAL_H_TWMNT2EJ */
