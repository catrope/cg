//
//  Framework for a raytracer
//  File: matrix.h
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Authors:
//    Roan Kattouw
//    Jan Paul Posma
//
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//

#ifndef MATRIX_H
#define MATRIX_H

#include "triple.h"

class Matrix {
public:
	Vector r1, r2, r3;
	
	Matrix(Vector r1, Vector r2, Vector r3) : r1(r1), r2(r2), r3(r3) {}
	
	Matrix transposed() const;
	Vector operator*(const Vector &v);
	Matrix operator*(const Matrix &m);
	
	static const Matrix rotation(const Vector &v, double angle);
	inline static const Matrix rotationDeg(const Vector &v, double angle) { return Matrix::rotation(v, angle*M_PI/180); }
};

#endif /* end of include guard: MATRIX_H */
